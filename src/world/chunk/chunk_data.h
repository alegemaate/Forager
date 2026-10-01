/// @file chunk_data.h
///
/// Block storage for one chunk, and the key that finds a chunk in the world
///
#pragma once

#include <array>
#include <cstddef>
#include <functional>

#include "../../block/block_type.h"
#include "../../core/Types.h"

using namespace core;

constexpr u32 CHUNK_WIDTH       = 16;
constexpr u32 CHUNK_HEIGHT      = 128;
constexpr u32 CHUNK_LENGTH      = 16;
constexpr u32 CHUNK_WIDTH_LOG2  = 4;
constexpr u32 CHUNK_LENGTH_LOG2 = 4;
constexpr u32 CHUNK_VOLUME      = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LENGTH;

/// @brief Water fills every open block at or below this height
constexpr i32 SEA_LEVEL = 58;

/// @brief Chunk position, in chunks
struct ChunkKey
{
    i32 x{0};
    i32 z{0};

    bool operator==(const ChunkKey&) const = default;
};

struct ChunkKeyHash
{
    size_t operator()(const ChunkKey& key) const noexcept
    {
        const u64 packed = (static_cast<u64>(static_cast<u32>(key.x)) << 32) | static_cast<u32>(key.z);
        return std::hash<u64>{}(packed);
    }
};

/// @brief Chunk that holds a world block position
inline ChunkKey chunkKeyAt(i32 x, i32 z)
{
    // Arithmetic shift floors, so -1 is in chunk -1
    return {x >> CHUNK_WIDTH_LOG2, z >> CHUNK_LENGTH_LOG2};
}

/// @brief One byte per block. Layout is x fastest, then z, then y, so a horizontal slice is contiguous.
struct ChunkData
{
    std::array<BlockID, CHUNK_VOLUME> blocks{};

    /// @brief Highest block that is not air
    u32 maxY{0};

    static constexpr u32 index(u32 x, u32 y, u32 z)
    {
        return x + (CHUNK_WIDTH * (z + (CHUNK_LENGTH * y)));
    }

    BlockID get(u32 x, u32 y, u32 z) const
    {
        return blocks[index(x, y, z)];
    }

    void set(u32 x, u32 y, u32 z, BlockID id)
    {
        blocks[index(x, y, z)] = id;
    }
};
