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

    void generate(World& world);

    void render(World& world);

    const BlockType& getTile(i32 x, i32 y, i32 z)
    {
        if (!inBounds(x, y, z))
        {
            return BlockRegistry::get(BlockID::Air);
        }

        return BlockRegistry::get(chunkAt(x, z).get(localX(x), static_cast<u32>(y), localZ(z)));
    }

    const BlockType& getTile(const glm::vec3& pos)
    {
        // Floor, so -0.5 maps to tile -1 and not 0
        return getTile(static_cast<i32>(std::floor(pos.x)), static_cast<i32>(std::floor(pos.y)),
                       static_cast<i32>(std::floor(pos.z)));
    }

    bool isSolidAt(i32 x, i32 y, i32 z) const noexcept
    {
        if (!inBounds(x, y, z))
        {
            return false;
        }

        return chunkAt(x, z).isSolidAt(localX(x), static_cast<u32>(y), localZ(z));
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
