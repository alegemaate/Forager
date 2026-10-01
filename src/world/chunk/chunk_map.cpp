#include "./chunk_map.h"

#include <algorithm>
#include <asw/asw.h>
#include <chrono>
#include <cmath>

#include "../../block/block_registry.h"
#include "../../render/frustum.h"
#include "../generator.h"

namespace
{
// Chunks loaded past the view distance, so every chunk in view has neighbours to light and mesh from
constexpr i32 LOAD_MARGIN = 2;

// Chunks kept past the load distance before they are dropped, so walking back and forth does not reload them
constexpr i32 UNLOAD_MARGIN = 2;

// Main thread time per frame for jobs, on web builds
constexpr auto WEB_JOB_BUDGET = std::chrono::milliseconds(8);

bool inRadius(i32 dx, i32 dz, i32 radius)
{
    return (dx * dx) + (dz * dz) <= (radius * radius) + radius;
}

i32 distanceSq(ChunkKey a, ChunkKey b)
{
    const i32 dx = a.x - b.x;
    const i32 dz = a.z - b.z;
    return (dx * dx) + (dz * dz);
}

ChunkKey keyAt(const glm::vec3& pos)
{
    // Blocks are centred on whole numbers
    return chunkKeyAt(static_cast<i32>(std::floor(pos.x + 0.5f)), static_cast<i32>(std::floor(pos.z + 0.5f)));
}

u32 localX(i32 x)
{
    return static_cast<u32>(x) & (CHUNK_WIDTH - 1);
}

u32 localZ(i32 z)
{
    return static_cast<u32>(z) & (CHUNK_LENGTH - 1);
}
} // namespace

ChunkMap::~ChunkMap()
{
    // Running jobs finish, queued ones are dropped
    jobs.clear();
    jobs.wait();
}

void ChunkMap::reset(u32 newSeed)
{
    clear();
    seed = newSeed;

    asw::log::info("New world, seed {}", seed);
}

void ChunkMap::clear()
{
    // Dropped jobs never report back, so wait for the running ones and start the counts again
    jobs.clear();
    jobs.wait();
    {
        const std::lock_guard lock(resultMutex);
        genResults.clear();
        meshResults.clear();
    }
    gensInFlight   = 0;
    meshesInFlight = 0;

    world++;
    chunks.clear();
    edits.clear();
}

Chunk* ChunkMap::find(ChunkKey key)
{
    const auto it = chunks.find(key);
    return it == chunks.end() ? nullptr : it->second.get();
}

const Chunk* ChunkMap::find(ChunkKey key) const
{
    const auto it = chunks.find(key);
    return it == chunks.end() ? nullptr : it->second.get();
}

void ChunkMap::stream(const glm::vec3& center, i32 radius)
{
    jobs.runOnMainThread(WEB_JOB_BUDGET);

    collectResults();

    const ChunkKey centerKey = keyAt(center);
    unloadFar(centerKey, radius + LOAD_MARGIN + UNLOAD_MARGIN);
    queueGeneration(centerKey, radius + LOAD_MARGIN);
    queueMeshes(centerKey, radius);
}

void ChunkMap::collectResults()
{
    std::vector<GenResult>  gens;
    std::vector<MeshResult> meshes;
    {
        const std::lock_guard lock(resultMutex);
        gens.swap(genResults);
        meshes.swap(meshResults);
    }

    gensInFlight -= gens.size();
    meshesInFlight -= meshes.size();

    for (auto& result : gens)
    {
        Chunk* chunk = find(result.key);
        if (result.world != world || chunk == nullptr)
        {
            continue;
        }

        // Put back what the player changed
        if (const auto it = edits.find(result.key); it != edits.end())
        {
            for (const auto& [index, id] : it->second)
            {
                result.data->blocks[index] = id;
                if (id != BlockID::Air)
                {
                    result.data->maxY = std::max(result.data->maxY, index / (CHUNK_WIDTH * CHUNK_LENGTH));
                }
            }
        }

        chunk->data = std::move(result.data);
        chunk->version++;
    }

    for (auto& result : meshes)
    {
        Chunk* chunk = find(result.key);
        if (result.world != world || chunk == nullptr)
        {
            continue;
        }

        chunk->meshQueued = false;
        chunk->mesh.upload(result.data);
        chunk->meshedVersion = result.version;
    }
}

void ChunkMap::unloadFar(ChunkKey center, i32 radius)
{
    std::erase_if(chunks,
                  [&](const auto& entry)
                  {
                      const auto& key = entry.first;
                      return !inRadius(key.x - center.x, key.z - center.z, radius);
                  });
}

void ChunkMap::queueGeneration(ChunkKey center, i32 radius)
{
    const size_t limit = JobQueue::workerCount() * 2;
    if (gensInFlight >= limit)
    {
        return;
    }

    // Missing chunks, nearest first
    std::vector<ChunkKey> missing;
    for (i32 dz = -radius; dz <= radius; dz++)
    {
        for (i32 dx = -radius; dx <= radius; dx++)
        {
            const ChunkKey key{center.x + dx, center.z + dz};
            if (inRadius(dx, dz, radius) && !chunks.contains(key))
            {
                missing.push_back(key);
            }
        }
    }

    std::ranges::sort(missing, {}, [&](ChunkKey key) { return distanceSq(key, center); });

    for (const auto& key : missing)
    {
        if (gensInFlight >= limit)
        {
            break;
        }

        chunks.emplace(key, std::make_unique<Chunk>(key));
        gensInFlight++;

        jobs.submit(
            [this, key, jobWorld = world, jobSeed = seed]()
            {
                auto data = generator::generateChunk(key, jobSeed);

                const std::lock_guard lock(resultMutex);
                genResults.push_back({key, jobWorld, std::move(data)});
            });
    }
}

bool ChunkMap::neighboursGenerated(ChunkKey key) const
{
    for (i32 dz = -1; dz <= 1; dz++)
    {
        for (i32 dx = -1; dx <= 1; dx++)
        {
            const Chunk* chunk = find({key.x + dx, key.z + dz});
            if (chunk == nullptr || !chunk->isGenerated())
            {
                return false;
            }
        }
    }
    return true;
}

void ChunkMap::queueMeshes(ChunkKey center, i32 radius)
{
    const size_t limit = JobQueue::workerCount() * 2;
    if (meshesInFlight >= limit)
    {
        return;
    }

    std::vector<Chunk*> waiting;
    for (auto& [key, chunk] : chunks)
    {
        if (chunk->needsMesh() && inRadius(key.x - center.x, key.z - center.z, radius) && neighboursGenerated(key))
        {
            waiting.push_back(chunk.get());
        }
    }

    // Edits first, then nearest
    std::ranges::sort(waiting, {}, [&](const Chunk* chunk)
                      { return std::pair(!chunk->urgent, distanceSq(chunk->getKey(), center)); });

    for (Chunk* chunk : waiting)
    {
        if (meshesInFlight >= limit)
        {
            break;
        }

        chunk->meshQueued = true;
        chunk->urgent     = false;
        meshesInFlight++;

        jobs.submit(
            [this, input = buildMeshInput(*chunk), jobWorld = world]()
            {
                auto data = mesher::buildMesh(input);

                const std::lock_guard lock(resultMutex);
                meshResults.push_back({input.key, jobWorld, input.version, std::move(data)});
            });
    }
}

MeshInput ChunkMap::buildMeshInput(const Chunk& chunk) const
{
    const ChunkKey key = chunk.getKey();

    MeshInput input;
    input.key     = key;
    input.version = chunk.version;

    // Copy up to just above the highest block, the rest is open air
    u32 maxY = 0;
    for (i32 dz = -1; dz <= 1; dz++)
    {
        for (i32 dx = -1; dx <= 1; dx++)
        {
            maxY = std::max(maxY, find({key.x + dx, key.z + dz})->data->maxY);
        }
    }
    input.sizeY = static_cast<i32>(std::min(maxY + 2, CHUNK_HEIGHT));
    input.blocks.assign(static_cast<size_t>(MESH_PW) * MESH_PL * input.sizeY, BlockID::Air);

    for (i32 dz = -1; dz <= 1; dz++)
    {
        for (i32 dx = -1; dx <= 1; dx++)
        {
            const ChunkData& data = *find({key.x + dx, key.z + dz})->data;

            // Padded position of this chunk's first block
            const i32 offsetX = (dx * static_cast<i32>(CHUNK_WIDTH)) + LIGHT_PAD;
            const i32 offsetZ = (dz * static_cast<i32>(CHUNK_LENGTH)) + LIGHT_PAD;

            // Part of this chunk inside the padded area
            const i32 startX = std::max(0, -offsetX);
            const i32 endX   = std::min(static_cast<i32>(CHUNK_WIDTH), MESH_PW - offsetX);
            const i32 startZ = std::max(0, -offsetZ);
            const i32 endZ   = std::min(static_cast<i32>(CHUNK_LENGTH), MESH_PL - offsetZ);

            for (i32 y = 0; y < input.sizeY; y++)
            {
                for (i32 z = startZ; z < endZ; z++)
                {
                    const auto* from = &data.blocks[ChunkData::index(startX, y, z)];
                    auto*       to   = &input.blocks[MeshInput::index(offsetX + startX, y, offsetZ + z)];
                    std::copy_n(from, endX - startX, to);
                }
            }
        }
    }

    return input;
}

void ChunkMap::render(const GpuProgram& shader, const glm::mat4& viewProjection, const glm::vec3& cameraPos) const
{
    const GLint modelLocation = shader.getUniformLocation("model");
    const GLint waterLocation = shader.getUniformLocation("uWater");

    const Frustum             frustum(viewProjection);
    std::vector<const Chunk*> visible;

    for (const auto& [key, chunk] : chunks)
    {
        if (chunk->hasMesh() && !chunk->mesh.empty() && frustum.intersects(chunk->getMin(), chunk->getMax()))
        {
            visible.push_back(chunk.get());
        }
    }

    ChunkMesh::bindAtlas();

    // Solid geometry
    glUniform1i(waterLocation, 0);
    for (const Chunk* chunk : visible)
    {
        chunk->mesh.renderOpaque(modelLocation, chunk->getOrigin());
    }

    // Water, far to near so it blends over what is behind it
    std::erase_if(visible, [](const Chunk* chunk) { return !chunk->mesh.hasWater(); });

    const auto distance = [&](const Chunk* chunk)
    {
        const glm::vec3 middle = chunk->getOrigin() + glm::vec3(CHUNK_WIDTH / 2.0f, 0.0f, CHUNK_LENGTH / 2.0f);
        const glm::vec2 flat(middle.x - cameraPos.x, middle.z - cameraPos.z);
        return glm::dot(flat, flat);
    };
    std::ranges::sort(visible, std::greater{}, distance);

    glUniform1i(waterLocation, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    for (const Chunk* chunk : visible)
    {
        chunk->mesh.renderWater(modelLocation, chunk->getOrigin());
    }

    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glUniform1i(waterLocation, 0);

    glBindVertexArray(0);
}

BlockID ChunkMap::getBlock(i32 x, i32 y, i32 z) const
{
    if (y < 0)
    {
        return BlockID::Stone;
    }
    if (y >= static_cast<i32>(CHUNK_HEIGHT))
    {
        return BlockID::Air;
    }

    const Chunk* chunk = find(chunkKeyAt(x, z));
    if (chunk == nullptr || !chunk->isGenerated())
    {
        return BlockID::Air;
    }

    return chunk->data->get(localX(x), static_cast<u32>(y), localZ(z));
}

bool ChunkMap::isLoadedAt(i32 x, i32 z) const
{
    const Chunk* chunk = find(chunkKeyAt(x, z));
    return chunk != nullptr && chunk->isGenerated();
}

bool ChunkMap::isSolidAt(i32 x, i32 y, i32 z) const
{
    if (!isLoadedAt(x, z))
    {
        return true;
    }
    return BlockRegistry::get(getBlock(x, y, z)).isSolid();
}

bool ChunkMap::setBlock(const glm::ivec3& pos, BlockID id)
{
    if (pos.y < 0 || pos.y >= static_cast<i32>(CHUNK_HEIGHT))
    {
        return false;
    }

    const ChunkKey key   = chunkKeyAt(pos.x, pos.z);
    Chunk*         chunk = find(key);
    if (chunk == nullptr || !chunk->isGenerated())
    {
        return false;
    }

    const u32 index            = ChunkData::index(localX(pos.x), static_cast<u32>(pos.y), localZ(pos.z));
    chunk->data->blocks[index] = id;
    edits[key][index]          = id;

    if (id != BlockID::Air)
    {
        chunk->data->maxY = std::max(chunk->data->maxY, static_cast<u32>(pos.y));
    }

    // Light and ambient occlusion reach into the chunks around
    for (i32 dz = -1; dz <= 1; dz++)
    {
        for (i32 dx = -1; dx <= 1; dx++)
        {
            if (Chunk* near = find({key.x + dx, key.z + dz}); near != nullptr && near->isGenerated())
            {
                near->version++;
                near->urgent = true;
            }
        }
    }

    return true;
}

i32 ChunkMap::getSurfaceY(i32 x, i32 z) const
{
    if (!isLoadedAt(x, z))
    {
        return -1;
    }

    for (i32 y = CHUNK_HEIGHT - 1; y >= 0; y--)
    {
        if (BlockRegistry::get(getBlock(x, y, z)).isSolid())
        {
            return y;
        }
    }
    return -1;
}

f32 ChunkMap::getReadiness(const glm::vec3& center, i32 radius) const
{
    const ChunkKey centerKey = keyAt(center);

    u32 total = 0;
    u32 ready = 0;
    for (i32 dz = -radius; dz <= radius; dz++)
    {
        for (i32 dx = -radius; dx <= radius; dx++)
        {
            if (!inRadius(dx, dz, radius))
            {
                continue;
            }

            total++;
            const Chunk* chunk = find({centerKey.x + dx, centerKey.z + dz});
            if (chunk != nullptr && chunk->hasMesh())
            {
                ready++;
            }
        }
    }

    return total == 0 ? 1.0f : static_cast<f32>(ready) / static_cast<f32>(total);
}
