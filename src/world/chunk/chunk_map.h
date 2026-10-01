/*
  Tile Map
  Allan Legemaate
  11/11/15
  Manages all the tiles
*/

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "../../core/JobQueue.h"
#include "../../core/Types.h"
#include "../../render/gpu_program.h"
#include "../../render/mesher.h"
#include "./chunk.h"

using namespace core;

/// @brief Endless world of chunks. Chunks around the player are generated and meshed in the background, and chunks
/// far away are dropped. Player edits are kept, so a dropped chunk comes back as the player left it.
class ChunkMap
{
  public:
    ChunkMap() = default;
    ~ChunkMap();

    ChunkMap(const ChunkMap&)            = delete;
    ChunkMap& operator=(const ChunkMap&) = delete;

    /// @brief Drop every chunk and edit and start a new world
    void reset(u32 newSeed);

    /// @brief Drop every chunk and edit
    void clear();

    /// @brief Load, mesh and drop chunks around a point. Call once per frame on the GL thread.
    ///
    /// @param center Point to load around, in blocks
    /// @param radius View distance, in chunks
    void stream(const glm::vec3& center, i32 radius);

    /// @brief Draw solid chunks, then water. The caller activates the shader and sets the shared uniforms.
    void render(const GpuProgram& shader, const glm::mat4& viewProjection, const glm::vec3& cameraPos) const;

    /// @brief Block at a position. Air when the chunk is not loaded, stone below the world.
    BlockID getBlock(i32 x, i32 y, i32 z) const;

    BlockID getBlock(const glm::ivec3& pos) const
    {
        return getBlock(pos.x, pos.y, pos.z);
    }

    /// @brief Check if a block stops the player. Chunks that are not loaded are solid, so nothing falls out.
    bool isSolidAt(i32 x, i32 y, i32 z) const;

    /// @brief Check if the chunk holding a position is generated
    bool isLoadedAt(i32 x, i32 z) const;

    /// @brief Change a block. Fails when the chunk is not loaded or the height is out of the world.
    bool setBlock(const glm::ivec3& pos, BlockID id);

    /// @brief Highest solid block in a column, or -1 when not loaded
    i32 getSurfaceY(i32 x, i32 z) const;

    /// @brief Share of chunks within a radius that have a mesh, 0 to 1
    f32 getReadiness(const glm::vec3& center, i32 radius) const;

    u32 getSeed() const
    {
        return seed;
    }

    /// @brief Chunks held in memory
    size_t getChunkCount() const
    {
        return chunks.size();
    }

  private:
    using ChunkTable = std::unordered_map<ChunkKey, std::unique_ptr<Chunk>, ChunkKeyHash>;
    using EditTable  = std::unordered_map<ChunkKey, std::unordered_map<u32, BlockID>, ChunkKeyHash>;

    struct GenResult
    {
        ChunkKey                   key;
        u64                        world;
        std::unique_ptr<ChunkData> data;
    };

    struct MeshResult
    {
        ChunkKey key;
        u64      world;
        u32      version;
        MeshData data;
    };

    Chunk*       find(ChunkKey key);
    const Chunk* find(ChunkKey key) const;

    void      collectResults();
    void      unloadFar(ChunkKey center, i32 radius);
    void      queueGeneration(ChunkKey center, i32 radius);
    void      queueMeshes(ChunkKey center, i32 radius);
    bool      neighboursGenerated(ChunkKey key) const;
    MeshInput buildMeshInput(const Chunk& chunk) const;

    u32 seed{0};

    /// @brief Bumped on reset, so jobs from the old world are thrown away
    u64 world{0};

    ChunkTable chunks;
    EditTable  edits;

    // Jobs running, counted on the main thread
    size_t gensInFlight{0};
    size_t meshesInFlight{0};

    // Finished jobs, filled by workers
    std::mutex              resultMutex;
    std::vector<GenResult>  genResults;
    std::vector<MeshResult> meshResults;

    // Last, so it is destroyed first and no job outlives the results it writes to
    JobQueue jobs;
};
