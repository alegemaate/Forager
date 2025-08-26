#pragma once

#include <glm/glm.hpp>

#include "../core/SimplexNoise.h"
#include "../core/Types.h"
#include "./ChunkMesh.h"
#include "./CubeFaces.h"
#include "./TileTypeManager.h"
#include "./Voxel.h"

class World;

class Chunk
{
  public:
    Chunk(u32 x, u32 z);

    // Generate chunk voxels
    void generate(World& world, u32 seed);

    // Get block
    Voxel& get(u32 x, u32 y, u32 z);

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

    Voxel blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]{};
    bool  changed = false;

    u32 height_map[CHUNK_WIDTH][CHUNK_LENGTH]{};

    // Data
    ChunkMesh mesh;
};
