#include "ChunkMap.h"

#include <asw/asw.h>
#include <stdexcept>

#include "../core/Logger.h"
#include "../utils/utils.h"
#include "./TileTypeManager.h"
#include "./World.h"

constexpr size_t WORLD_WIDTH = 8;
constexpr size_t WORLD_LENGTH = 8;

// Update map
void ChunkMap::update(World& world) {
  for (auto& chunk : chunks) {
    chunk->update(world);
  }
}

// Procedural Generation of map
void ChunkMap::generate(World& world) {
  // GENERATE MAP
  Logger::heading("Generating Map");

  // Set empty tile
  emptyTile.setType(world.getTileManager().getTileByType(TileID::Air));

  // Clear chunks
  chunks.clear();

  auto seed = random(0, 10000);

  int currentChunk = 0;
  const int worldSize = WORLD_WIDTH * WORLD_LENGTH;

  // Make lots of chunks
  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int j = 0; j < WORLD_LENGTH; j++) {
      auto& chunk = chunks.emplace_back(std::make_unique<Chunk>(i, j));
      chunk->generate(world, seed);
      currentChunk++;

      // Send to console
      Logger::progress(
          std::to_string(currentChunk) + "/" + std::to_string(worldSize),
          static_cast<float>(currentChunk) / worldSize);
    }
  }
}

// Draw map
void ChunkMap::render(World& world) {
  const auto& defaultShader = world.getGpuProgramManager().getShader("default");
  const auto& camera = world.getCamera();
  const auto& lightDir = world.getLightDir();
  const auto& lightColor = world.getLightColor();
  const auto& lightAmbient = world.getLightAmbient();

  // Activate shader
  defaultShader.activate();
  defaultShader.setMat4("projection", camera.getProjectionMatrix());
  defaultShader.setMat4("view", camera.getViewMatrix());
  defaultShader.setVec3("light.direction", lightDir);
  defaultShader.setVec3("light.ambient", lightAmbient);
  defaultShader.setVec3("light.color", lightColor);

  // Cube map
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 1);

  for (auto& chunk : chunks) {
    chunk->render(world);
  }

  // Deactivate shader
  defaultShader.deactivate();
}

Voxel& ChunkMap::getTile(unsigned int x, unsigned int y, unsigned int z) {
  const auto chunkX = x / CHUNK_WIDTH;
  const auto chunkZ = z / CHUNK_LENGTH;

  if (chunkX >= WORLD_WIDTH || chunkZ >= WORLD_LENGTH) {
    return emptyTile;
  }

  auto index = chunkX * WORLD_LENGTH + chunkZ;

  if (index < chunks.size()) {
    return chunks[index]->get(x % CHUNK_WIDTH, y % CHUNK_HEIGHT,
                              z % CHUNK_LENGTH);
  } else {
    throw std::out_of_range("Chunk index out of range");
  }

  return emptyTile;  // Return an empty tile if not found
}