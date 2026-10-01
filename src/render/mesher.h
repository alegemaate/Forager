/// @file mesher.h
///
/// Builds chunk geometry and lighting on worker threads. The input is a copy of the chunk and the blocks around it,
/// so a job never reads world state that the main thread changes.
///
#pragma once

#include <vector>

#include "../world/chunk/chunk_data.h"

/// @brief Blocks copied around the chunk. Light travels this far, so light from neighbours is right at the edges.
constexpr i32 LIGHT_PAD = 14;
constexpr i32 MESH_PW   = CHUNK_WIDTH + (2 * LIGHT_PAD);
constexpr i32 MESH_PL   = CHUNK_LENGTH + (2 * LIGHT_PAD);

/// @brief Brightest light. Open sky is this bright.
constexpr u8 MAX_LIGHT = 15;

/// @brief Floats per vertex: position 3, normal 3, uv 2, ambient occlusion 1, sky light 1, block light 1
constexpr u32 VERTEX_FLOATS = 11;

struct MeshInput
{
    ChunkKey key;
    u32      version{0};

    /// @brief Layers copied. Everything above is open air.
    i32 sizeY{0};

    /// @brief MESH_PW * MESH_PL * sizeY blocks, x fastest, then z, then y
    std::vector<BlockID> blocks;

    static size_t index(i32 x, i32 y, i32 z)
    {
        return static_cast<size_t>(x + (MESH_PW * (z + (MESH_PL * y))));
    }
};

struct MeshData
{
    std::vector<f32> vertices;
    std::vector<u32> indices;

    /// @brief Indices of solid geometry come first, then water
    u32 opaqueIndices{0};
    u32 waterIndices{0};
};

namespace mesher
{

MeshData buildMesh(const MeshInput& input);

} // namespace mesher
