/*
  Tile Map
  Allan Legemaate
  11/11/15
  Manages all the tiles
*/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../core/ThreadPool.h"
#include "../core/Types.h"
#include "./BiomeManager.h"
#include "./Chunk.h"
#include "./TileTypeManager.h"
#include "./Voxel.h"

using namespace core;

class World;

class ChunkMap
{
  public:
    void update(World& world);

    void generate(World& world);

    void render(World& world);

    Voxel& getTile(u32 x, u32 y, u32 z);

    Voxel& getTile(const glm::vec3& pos)
    {
        return getTile(static_cast<u32>(pos.x), static_cast<u32>(pos.y), static_cast<u32>(pos.z));
    }

    bool isSolidAt(u32 x, u32 y, u32 z)
    {
        auto& tile = getTile(x, y, z);
        return tile.isSolid();
    }

  private:
    // All chunks
    std::vector<Chunk> chunks;

    // Default tile
    Voxel emptyTile;
};
