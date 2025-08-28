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

#include "../../block/block.h"
#include "../../block/block_registry.h"
#include "../../core/ThreadPool.h"
#include "../../core/Types.h"
#include "../biome/biome_registry.h"
#include "./chunk.h"

using namespace core;

constexpr size_t WORLD_WIDTH  = 8;
constexpr size_t WORLD_LENGTH = 8;

class World;

class ChunkMap
{
  public:
    void update(World& world);

    void generate(World& world);

    void render(World& world);

    Block& getTile(u32 x, u32 y, u32 z)
    {
        if (y >= CHUNK_HEIGHT)
        {
            return emptyTile;
        }

        const u32 cx = x >> CHUNK_WIDTH_LOG2;
        const u32 cz = z >> CHUNK_LENGTH_LOG2;
        if (cx >= WORLD_WIDTH || cz >= WORLD_LENGTH)
        {
            return emptyTile;
        }

        Chunk&    c  = chunks[(cx * WORLD_LENGTH) + cz];
        const u32 lx = x & (CHUNK_WIDTH - 1);
        const u32 ly = y;
        const u32 lz = z & (CHUNK_LENGTH - 1);

        return c.get(lx, ly, lz);
    }

    Block& getTile(const glm::vec3& pos)
    {
        return getTile(static_cast<u32>(pos.x), static_cast<u32>(pos.y), static_cast<u32>(pos.z));
    }

    bool isSolidAt(u32 x, u32 y, u32 z) const noexcept
    {
        if (y >= CHUNK_HEIGHT)
        {
            return false;
        }

        const u32 cx = x >> CHUNK_WIDTH_LOG2;
        const u32 cz = z >> CHUNK_LENGTH_LOG2;
        if (cx >= WORLD_WIDTH || cz >= WORLD_LENGTH)
        {
            return false;
        }

        const Chunk& c  = chunks[(cx * WORLD_LENGTH) + cz];
        const u32    lx = x & (CHUNK_WIDTH - 1);
        const u32    ly = y;
        const u32    lz = z & (CHUNK_LENGTH - 1);

        return c.isSolidAt(lx, ly, lz);
    }

  private:
    // All chunks
    std::vector<Chunk> chunks;

    // Default tile
    Block emptyTile;
};
