/// @file generator.h
///
/// Terrain generation. Pure functions of the seed and chunk position, so they run on worker threads.
///
#pragma once

#include <memory>

#include "./chunk/chunk_data.h"

namespace generator
{

/// @brief Build the blocks of one chunk
std::unique_ptr<ChunkData> generateChunk(ChunkKey key, u32 seed);

} // namespace generator
