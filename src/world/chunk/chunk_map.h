/*
  Tile Map
  Allan Legemaate
  11/11/15
  Manages all the tiles
*/

#pragma once

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "../../block/block_registry.h"
#include "../../core/JobQueue.h"
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

    void generate(World& world, u32 seed);

    // Drop every chunk
    void clear()
    {
        chunks.clear();
    }

    void render(World& world);

    /// @brief Block at a position. Air outside the map, stone below the world.
    BlockID getBlock(i32 x, i32 y, i32 z) const
    {
        if (y < 0)
        {
            return BlockID::Stone;
        }
        if (!inBounds(x, y, z))
        {
            return BlockID::Air;
        }

        return chunkAt(x, z).get(localX(x), static_cast<u32>(y), localZ(z));
    }

    /// @brief Check if a block stops the player. Outside the map and below the world are solid, so the edge is a
    /// wall and nothing falls out.
    bool isSolidAt(i32 x, i32 y, i32 z) const noexcept
    {
        if (y >= static_cast<i32>(CHUNK_HEIGHT))
        {
            return false;
        }
        if (!inBounds(x, y, z))
        {
            return true;
        }

        return chunkAt(x, z).isSolidAt(localX(x), static_cast<u32>(y), localZ(z));
    }

    /// @brief Check if a column is inside the map
    bool isLoadedAt(i32 x, i32 z) const noexcept
    {
        return inBounds(x, 0, z);
    }

    /// @brief Highest solid block in a column, or -1 outside the map
    i32 getSurfaceY(i32 x, i32 z) const
    {
        if (!isLoadedAt(x, z))
        {
            return -1;
        }

        for (i32 y = CHUNK_HEIGHT - 1; y >= 0; y--)
        {
            if (isSolidAt(x, y, z))
            {
                return y;
            }
        }
        return -1;
    }

  private:
    // All chunks
    std::vector<Chunk> chunks;

    static bool inBounds(i32 x, i32 y, i32 z) noexcept
    {
        return x >= 0 && y >= 0 && z >= 0 && static_cast<u32>(y) < CHUNK_HEIGHT &&
               (static_cast<u32>(x) >> CHUNK_WIDTH_LOG2) < WORLD_WIDTH &&
               (static_cast<u32>(z) >> CHUNK_LENGTH_LOG2) < WORLD_LENGTH;
    }

    static u32 localX(i32 x) noexcept
    {
        return static_cast<u32>(x) & (CHUNK_WIDTH - 1);
    }

    static u32 localZ(i32 z) noexcept
    {
        return static_cast<u32>(z) & (CHUNK_LENGTH - 1);
    }

    // Caller checks inBounds first
    Chunk& chunkAt(i32 x, i32 z)
    {
        return chunks[((static_cast<u32>(x) >> CHUNK_WIDTH_LOG2) * WORLD_LENGTH) +
                      (static_cast<u32>(z) >> CHUNK_LENGTH_LOG2)];
    }

    const Chunk& chunkAt(i32 x, i32 z) const
    {
        return chunks[((static_cast<u32>(x) >> CHUNK_WIDTH_LOG2) * WORLD_LENGTH) +
                      (static_cast<u32>(z) >> CHUNK_LENGTH_LOG2)];
    }
};
