#include "./mesher.h"

#include <array>

#include "../block/block_registry.h"
#include "./cube_faces.h"

namespace
{
constexpr u32 ATLAS_WIDTH     = 8;
constexpr f32 ATLAS_WIDTH_INV = 1.0f / ATLAS_WIDTH;

// Water surface sits a little below the top of its block
constexpr f32 WATER_DROP = 0.1f;

// Torch stick size, as a scale of a full block
constexpr f32 TORCH_WIDTH  = 0.125f;
constexpr f32 TORCH_HEIGHT = 0.625f;

constexpr std::array<glm::ivec3, 6> DIRECTIONS = {
    glm::ivec3(1, 0, 0),  glm::ivec3(-1, 0, 0), glm::ivec3(0, 1, 0),
    glm::ivec3(0, -1, 0), glm::ivec3(0, 0, 1),  glm::ivec3(0, 0, -1),
};

// Block properties copied into flat tables, looked up for every block
struct BlockFlags
{
    std::array<bool, 256> opaque{};
    std::array<bool, 256> liquid{};
    std::array<u8, 256>   light{};

    BlockFlags()
    {
        for (u32 i = 0; i < static_cast<u32>(BlockID::Max); i++)
        {
            const auto& type = BlockRegistry::get(static_cast<BlockID>(i));
            opaque[i]        = type.isOpaque();
            liquid[i]        = type.isLiquid();
            light[i]         = type.getLight();
        }
    }
};

struct Face
{
    const FaceDefinition& def;
    unsigned int AtlasLookup::* atlas;
};

const std::array<Face, 6>& faces()
{
    static const std::array<Face, 6> list = {
        Face{topFace, &AtlasLookup::top},     Face{bottomFace, &AtlasLookup::bottom},
        Face{leftFace, &AtlasLookup::left},   Face{rightFace, &AtlasLookup::right},
        Face{frontFace, &AtlasLookup::front}, Face{backFace, &AtlasLookup::back},
    };
    return list;
}

class Builder
{
  public:
    explicit Builder(const MeshInput& input)
        : in(input), cells(static_cast<size_t>(MESH_PW) * MESH_PL * input.sizeY), sky(cells, 0), block(cells, 0)
    {
    }

    MeshData build()
    {
        lightSky();
        lightBlocks();
        tessellate();

        // Water after the solid geometry, in the same buffers
        MeshData data;
        data.opaqueIndices = static_cast<u32>(solid.indices.size());
        data.waterIndices  = static_cast<u32>(water.indices.size());

        const u32 waterBase = static_cast<u32>(solid.vertices.size() / VERTEX_FLOATS);
        data.vertices       = std::move(solid.vertices);
        data.vertices.insert(data.vertices.end(), water.vertices.begin(), water.vertices.end());

        data.indices = std::move(solid.indices);
        data.indices.reserve(data.indices.size() + water.indices.size());
        for (const u32 index : water.indices)
        {
            data.indices.push_back(waterBase + index);
        }

        return data;
    }

  private:
    struct Geometry
    {
        std::vector<f32> vertices;
        std::vector<u32> indices;
    };

    const MeshInput& in;
    BlockFlags       flags;
    size_t           cells;
    std::vector<u8>  sky;
    std::vector<u8>  block;
    std::vector<u32> queue;
    Geometry         solid;
    Geometry         water;

    bool inside(i32 x, i32 y, i32 z) const
    {
        return x >= 0 && x < MESH_PW && z >= 0 && z < MESH_PL && y >= 0 && y < in.sizeY;
    }

    // Above the copy is air, below the world is stone
    BlockID at(i32 x, i32 y, i32 z) const
    {
        if (y >= in.sizeY)
        {
            return BlockID::Air;
        }
        if (y < 0 || x < 0 || x >= MESH_PW || z < 0 || z >= MESH_PL)
        {
            return BlockID::Stone;
        }
        return in.blocks[MeshInput::index(x, y, z)];
    }

    bool opaqueAt(i32 x, i32 y, i32 z) const
    {
        return flags.opaque[static_cast<u8>(at(x, y, z))];
    }

    u8 skyAt(i32 x, i32 y, i32 z) const
    {
        if (y >= in.sizeY)
        {
            return MAX_LIGHT;
        }
        return inside(x, y, z) ? sky[MeshInput::index(x, y, z)] : 0;
    }

    u8 blockAt(i32 x, i32 y, i32 z) const
    {
        return inside(x, y, z) ? block[MeshInput::index(x, y, z)] : 0;
    }

    // Spread light from the queued cells. Each step costs 1, water costs 2.
    void propagate(std::vector<u8>& light)
    {
        for (size_t head = 0; head < queue.size(); head++)
        {
            const u32 i     = queue[head];
            const u8  level = light[i];
            const i32 x     = static_cast<i32>(i % MESH_PW);
            const i32 z     = static_cast<i32>((i / MESH_PW) % MESH_PL);
            const i32 y     = static_cast<i32>(i / (MESH_PW * MESH_PL));

            for (const auto& dir : DIRECTIONS)
            {
                const i32 nx = x + dir.x;
                const i32 ny = y + dir.y;
                const i32 nz = z + dir.z;

                if (!inside(nx, ny, nz))
                {
                    continue;
                }

                const size_t n  = MeshInput::index(nx, ny, nz);
                const auto   id = static_cast<u8>(in.blocks[n]);
                if (flags.opaque[id])
                {
                    continue;
                }

                const u8 cost = flags.liquid[id] ? 2 : 1;
                if (level > cost && level - cost > light[n])
                {
                    light[n] = level - cost;
                    queue.push_back(static_cast<u32>(n));
                }
            }
        }
        queue.clear();
    }

    void lightSky()
    {
        // Straight down from the sky, until something opaque
        for (i32 z = 0; z < MESH_PL; z++)
        {
            for (i32 x = 0; x < MESH_PW; x++)
            {
                u8 level = MAX_LIGHT;
                for (i32 y = in.sizeY - 1; y >= 0; y--)
                {
                    const size_t i  = MeshInput::index(x, y, z);
                    const auto   id = static_cast<u8>(in.blocks[i]);

                    if (flags.opaque[id])
                    {
                        level = 0;
                    }
                    else if (flags.liquid[id])
                    {
                        level = level > 2 ? level - 2 : 0;
                    }

                    sky[i] = level;
                }
            }
        }

        // Then sideways into overhangs and caves, from lit cells next to darker ones
        for (i32 y = 0; y < in.sizeY; y++)
        {
            for (i32 z = 0; z < MESH_PL; z++)
            {
                for (i32 x = 0; x < MESH_PW; x++)
                {
                    const size_t i     = MeshInput::index(x, y, z);
                    const u8     level = sky[i];
                    if (level <= 1)
                    {
                        continue;
                    }

                    const bool darker =
                        (x > 0 && sky[i - 1] + 1 < level) || (x + 1 < MESH_PW && sky[i + 1] + 1 < level) ||
                        (z > 0 && sky[i - MESH_PW] + 1 < level) || (z + 1 < MESH_PL && sky[i + MESH_PW] + 1 < level) ||
                        (y > 0 && sky[i - (MESH_PW * MESH_PL)] + 1 < level);
                    if (darker)
                    {
                        queue.push_back(static_cast<u32>(i));
                    }
                }
            }
        }

        propagate(sky);
    }

    void lightBlocks()
    {
        for (size_t i = 0; i < cells; i++)
        {
            const u8 emit = flags.light[static_cast<u8>(in.blocks[i])];
            if (emit > 0)
            {
                block[i] = emit;
                queue.push_back(static_cast<u32>(i));
            }
        }

        propagate(block);
    }

    // Smooth light at a corner, from the open cells that touch it
    void cornerLight(const glm::ivec3& outside, const glm::ivec3& sideA, const glm::ivec3& sideB,
                     const glm::ivec3& corner, f32& outSky, f32& outBlock) const
    {
        const bool openA      = !opaqueAt(sideA.x, sideA.y, sideA.z);
        const bool openB      = !opaqueAt(sideB.x, sideB.y, sideB.z);
        const bool openCorner = (openA || openB) && !opaqueAt(corner.x, corner.y, corner.z);

        u32 skySum   = skyAt(outside.x, outside.y, outside.z);
        u32 blockSum = blockAt(outside.x, outside.y, outside.z);
        u32 count    = 1;

        const auto add = [&](bool open, const glm::ivec3& cell)
        {
            if (open)
            {
                skySum += skyAt(cell.x, cell.y, cell.z);
                blockSum += blockAt(cell.x, cell.y, cell.z);
                count++;
            }
        };

        add(openA, sideA);
        add(openB, sideB);
        add(openCorner, corner);

        outSky   = static_cast<f32>(skySum) / static_cast<f32>(count * MAX_LIGHT);
        outBlock = static_cast<f32>(blockSum) / static_cast<f32>(count * MAX_LIGHT);
    }

    static void pushVertex(Geometry& geo, const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& uv, f32 ao,
                           f32 skyLight, f32 blockLight)
    {
        geo.vertices.insert(geo.vertices.end(),
                            {pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, uv.x, uv.y, ao, skyLight, blockLight});
    }

    static void pushQuad(Geometry& geo, u32 baseIndex)
    {
        static constexpr u32 QUAD[6] = {0, 1, 2, 0, 2, 3};
        for (const u32 i : QUAD)
        {
            geo.indices.push_back(baseIndex + i);
        }
    }

    static glm::vec2 atlasUV(u32 atlasPos, u32 corner)
    {
        const u32 atlasX = atlasPos % ATLAS_WIDTH;
        const u32 atlasY = atlasPos / ATLAS_WIDTH;
        return {(faceUVs[corner].x + static_cast<f32>(atlasX)) * ATLAS_WIDTH_INV,
                (faceUVs[corner].y + static_cast<f32>(atlasY)) * ATLAS_WIDTH_INV};
    }

    void cubeFaces(BlockID id, const glm::ivec3& cell, const glm::vec3& base)
    {
        const auto& type     = BlockRegistry::get(id);
        const bool  isOpaque = type.isOpaque();
        const bool  isLiquid = type.isLiquid();
        auto&       geo      = isLiquid ? water : solid;

        // Lower the water surface when nothing sits on it
        const bool surface = isLiquid && at(cell.x, cell.y + 1, cell.z) != id;

        for (const auto& face : faces())
        {
            const glm::ivec3 outside   = cell + glm::ivec3(face.def.normal);
            const BlockID    neighbour = at(outside.x, outside.y, outside.z);

            // Solid blocks show faces that are not hidden. See through blocks also hide faces against their own kind.
            if (flags.opaque[static_cast<u8>(neighbour)] || (!isOpaque && neighbour == id))
            {
                continue;
            }

            const u32 atlasPos  = type.getAtlasIds().*face.atlas;
            const u32 baseIndex = static_cast<u32>(geo.vertices.size() / VERTEX_FLOATS);

            for (u32 i = 0; i < 4; i++)
            {
                const glm::ivec3 corner = cell + glm::ivec3(face.def.neighbours[(2 * i) % 8]);
                const glm::ivec3 sideA  = cell + glm::ivec3(face.def.neighbours[(2 * i + 1) % 8]);
                const glm::ivec3 sideB  = cell + glm::ivec3(face.def.neighbours[(2 * i + 7) % 8]);

                // "Hard corner" rule: if both sides are filled, corner doesn't matter
                const bool solidA      = opaqueAt(sideA.x, sideA.y, sideA.z);
                const bool solidB      = opaqueAt(sideB.x, sideB.y, sideB.z);
                const bool solidCorner = opaqueAt(corner.x, corner.y, corner.z);
                const int  occ         = (solidA && solidB) ? 3
                                                            : static_cast<int>(solidA) + static_cast<int>(solidB) +
                                                                  static_cast<int>(solidCorner);
                const f32  ao          = (3.0f - static_cast<f32>(occ)) / 3.0f;

                f32 skyLight   = 0.0f;
                f32 blockLight = 0.0f;
                cornerLight(outside, sideA, sideB, corner, skyLight, blockLight);

                glm::vec3 pos = face.def.vertices[i];
                if (surface && pos.y > 0.0f)
                {
                    pos.y -= WATER_DROP;
                }

                pushVertex(geo, pos + base, face.def.normal, atlasUV(atlasPos, i), ao, skyLight, blockLight);
            }

            pushQuad(geo, baseIndex);
        }
    }

    void torchFaces(BlockID id, const glm::ivec3& cell, const glm::vec3& base)
    {
        const auto& type     = BlockRegistry::get(id);
        const f32   skyLight = static_cast<f32>(skyAt(cell.x, cell.y, cell.z)) / MAX_LIGHT;

        for (const auto& face : faces())
        {
            const u32 atlasPos  = type.getAtlasIds().*face.atlas;
            const u32 baseIndex = static_cast<u32>(solid.vertices.size() / VERTEX_FLOATS);

            for (u32 i = 0; i < 4; i++)
            {
                const glm::vec3& v = face.def.vertices[i];
                const glm::vec3  pos(v.x * TORCH_WIDTH, ((v.y + 0.5f) * TORCH_HEIGHT) - 0.5f, v.z * TORCH_WIDTH);

                // Torches glow, so they are fully lit
                pushVertex(solid, pos + base, face.def.normal, atlasUV(atlasPos, i), 1.0f, skyLight, 1.0f);
            }

            pushQuad(solid, baseIndex);
        }
    }

    void tessellate()
    {
        for (i32 y = 0; y < in.sizeY; y++)
        {
            for (i32 z = LIGHT_PAD; z < LIGHT_PAD + static_cast<i32>(CHUNK_LENGTH); z++)
            {
                for (i32 x = LIGHT_PAD; x < LIGHT_PAD + static_cast<i32>(CHUNK_WIDTH); x++)
                {
                    const BlockID id = in.blocks[MeshInput::index(x, y, z)];
                    if (id == BlockID::Air)
                    {
                        continue;
                    }

                    const glm::ivec3 cell(x, y, z);
                    const glm::vec3  base(static_cast<f32>(x - LIGHT_PAD), static_cast<f32>(y),
                                          static_cast<f32>(z - LIGHT_PAD));

                    switch (BlockRegistry::get(id).getModel())
                    {
                    case BlockModel::Cube:
                        cubeFaces(id, cell, base);
                        break;
                    case BlockModel::Torch:
                        torchFaces(id, cell, base);
                        break;
                    case BlockModel::None:
                        break;
                    }
                }
            }
        }
    }
};
} // namespace

MeshData mesher::buildMesh(const MeshInput& input)
{
    return Builder(input).build();
}
