#pragma once

#include <glm/glm.hpp>

#include "../../block/block_registry.h"
#include "../../core/SimplexNoise.h"
#include "../../core/Types.h"
#include "../../render/chunk_mesh.h"
#include "../../render/cube_faces.h"

class World;

class Chunk
{
  public:
    Chunk(u32 x, u32 z);

    // Generate chunk voxels
    void generate(World& world, u32 seed);

    // Get block
    BlockID& get(u32 x, u32 y, u32 z);

    BlockID get(u32 x, u32 y, u32 z) const
    {
        return blk[x][y][z];
    }

    // Check if solid at
    bool isSolidAt(u32 x, u32 y, u32 z) const
    {
        return BlockRegistry::get(blk[x][y][z]).isSolid();
    }

    // Tessellate and such
    void update(World& world);

    // Render it all. The caller activates the shader and binds the atlas.
    void render(GLint modelLocation) const;

    // True when the mesh has nothing to draw
    bool empty() const
    {
        return mesh.empty();
    }

    // World space bounds, for culling. Cubes are centred on their block position.
    glm::vec3 getMin() const
    {
        return getOrigin() - glm::vec3(0.5f);
    }

    glm::vec3 getMax() const
    {
        return getOrigin() + glm::vec3(CHUNK_WIDTH, static_cast<f32>(maxHeight) + 1.0f, CHUNK_LENGTH) - glm::vec3(0.5f);
    }

    // Position
    u32 getX() const
    {
        return index_x;
    }

    u32 getZ() const
    {
        return index_z;
    }

  private:
    u32 index_x;
    u32 index_z;

    BlockID blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]{};
    bool    changed = false;

    u32 height_map[CHUNK_WIDTH][CHUNK_LENGTH]{};

    // Highest terrain block, nothing is placed above it
    u32 maxHeight = CHUNK_HEIGHT - 1;

    glm::vec3 getOrigin() const
    {
        return {static_cast<f32>(index_x * CHUNK_WIDTH), 0.0f, static_cast<f32>(index_z * CHUNK_LENGTH)};
    }

    // Data
    ChunkMesh mesh;
};
