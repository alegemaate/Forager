#pragma once

#include <glm/glm.hpp>

#include "../../block/block.h"
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
    Block& get(u32 x, u32 y, u32 z);

    // Check if solid at
    bool isSolidAt(u32 x, u32 y, u32 z) const
    {
        return blk[x][y][z].isSolid();
    }

    // Tessellate and such
    void update(World& world);

    // Render it all
    void render(World& world);

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

    Block blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]{};
    bool  changed = false;

    u32 height_map[CHUNK_WIDTH][CHUNK_LENGTH]{};

    // Data
    ChunkMesh mesh;
};
