#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "../../core/Types.h"
#include "../../render/chunk_mesh.h"
#include "./chunk_data.h"

/// @brief A column of blocks and its mesh. Owned and updated by the ChunkMap on the main thread.
class Chunk
{
  public:
    explicit Chunk(ChunkKey key) : key(key) {}

    ChunkKey getKey() const
    {
        return key;
    }

    /// @brief Blocks, or null while the chunk is being generated
    std::unique_ptr<ChunkData> data;

    ChunkMesh mesh;

    /// @brief Bumped on every change that needs a new mesh
    u32 version{1};

    /// @brief Version the current mesh was built from, 0 before the first mesh
    u32 meshedVersion{0};

    /// @brief A mesh job is running
    bool meshQueued{false};

    /// @brief Mesh before other chunks, for edits the player is waiting on
    bool urgent{false};

    bool isGenerated() const
    {
        return data != nullptr;
    }

    bool hasMesh() const
    {
        return meshedVersion != 0;
    }

    bool needsMesh() const
    {
        return isGenerated() && !meshQueued && version != meshedVersion;
    }

    glm::vec3 getOrigin() const
    {
        return {static_cast<f32>(key.x * static_cast<i32>(CHUNK_WIDTH)), 0.0f,
                static_cast<f32>(key.z * static_cast<i32>(CHUNK_LENGTH))};
    }

    // World space bounds, for culling. Cubes are centred on their block position.
    glm::vec3 getMin() const
    {
        return getOrigin() - glm::vec3(0.5f);
    }

    glm::vec3 getMax() const
    {
        const f32 top = data ? static_cast<f32>(data->maxY) + 1.0f : static_cast<f32>(CHUNK_HEIGHT);
        return getOrigin() + glm::vec3(CHUNK_WIDTH, top, CHUNK_LENGTH) - glm::vec3(0.5f);
    }

  private:
    ChunkKey key;
};
