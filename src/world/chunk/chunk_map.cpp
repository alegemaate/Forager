#include "./chunk_map.h"

#include <asw/asw.h>
#include <stdexcept>

#include "../../block/block_registry.h"
#include "../../utils/utils.h"
#include "../world.h"

using namespace std;

// Update map
void ChunkMap::update(World& world)
{
    for (auto& chunk : chunks)
    {
        chunk.update(world);
    }
}

// Procedural Generation of map
void ChunkMap::generate(World& world)
{
    // GENERATE MAP
    asw::log::info("Generating Map");

    // Set empty tile
    emptyTile.setType(world.getTileManager().getTileByType(BlockID::Air));

    // Clear chunks
    chunks.clear();

    const u32 seed         = 100; // asw::random::between(0, 10000);
    const i32 worldSize    = WORLD_WIDTH * WORLD_LENGTH;
    i32       currentChunk = 0;

    // Reserve space for chunks
    chunks.reserve(worldSize);

    // Thread pool for chunk generation
    ThreadPool threadPool;

    // Make lots of chunks
    for (u32 i = 0; i < WORLD_WIDTH; i++)
    {
        for (u32 j = 0; j < WORLD_LENGTH; j++)
        {
            auto& chunk = chunks.emplace_back(i, j);

            threadPool.enqueue([&chunk, &world]() { chunk.generate(world, seed); });
            currentChunk++;

            // Send to console
            asw::log::progress(static_cast<float>(currentChunk) / worldSize, "{} / {}", currentChunk, worldSize);
        }
    }

    // Wait for all tasks to finish
    threadPool.wait();

    asw::log::info("Map generation complete!");

    // Initial update
    for (auto& chunk : chunks)
    {
        chunk.update(world);
    }

    asw::log::info("Chunk update complete!");
}

// Draw map
void ChunkMap::render(World& world)
{
    const auto& defaultShader = world.getGpuProgramManager().getShader("default");
    const auto& camera        = world.getCamera();
    const auto& lightDir      = world.getLightDir();
    const auto& lightColor    = world.getLightColor();
    const auto& lightAmbient  = world.getLightAmbient();

    // Activate shader
    defaultShader.activate();
    defaultShader.setMat4("projection", camera.getProjectionMatrix());
    defaultShader.setMat4("view", camera.getViewMatrix());
    defaultShader.setVec3("light.direction", lightDir);
    defaultShader.setVec3("light.ambient", lightAmbient);
    defaultShader.setVec3("light.color", lightColor);

    for (auto& chunk : chunks)
    {
        chunk.render(world);
    }

    // Deactivate shader
    defaultShader.deactivate();
}
