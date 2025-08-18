#include "ChunkMap.h"

#include <asw/asw.h>
#include <stdexcept>

#include "../core/Logger.h"
#include "../utils/utils.h"
#include "./TileTypeManager.h"
#include "./World.h"

constexpr size_t WORLD_WIDTH = 4;
constexpr size_t WORLD_LENGTH = 4;
constexpr size_t WORLD_HEIGHT = 1;

// Update map
void ChunkMap::update(World& world) {
  for (auto& chunk : chunks) {
    chunk->update();
  }
}

// Procedural Generation of map
void ChunkMap::generate(TileTypeManager& tileManager) {
  // GENERATE MAP
  Logger::heading("Generating Map");

  // Set empty tile
  emptyTile.setType(tileManager.getTileByType(TileID::Air));

  // Clear chunks
  chunks.clear();

  auto seed = random(0, 10000);

  int currentChunk = 0;
  const int worldSize = WORLD_WIDTH * WORLD_LENGTH * WORLD_HEIGHT;

  // Make lots of chunks
  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int t = 0; t < WORLD_HEIGHT; t++) {
      for (unsigned int j = 0; j < WORLD_LENGTH; j++) {
        auto& chunk = chunks.emplace_back(std::make_unique<Chunk>(i, t, j));
        chunk->generate(tileManager, seed);
        currentChunk++;

        // Send to console
        Logger::progress(
            std::to_string(currentChunk) + "/" + std::to_string(worldSize),
            static_cast<float>(currentChunk) / worldSize);
      }
    }
  }
}

// Draw map
void ChunkMap::render(World& world) {
  const auto& defaultShader = world.getShaderManager().getShader("default");
  const auto& camera = world.getCamera();
  const auto& lightDir = world.getLightDir();
  const auto& lightColor = world.getLightColor();

  // Activate shader
  defaultShader.activate();
  defaultShader.setMat4("projection", camera.getProjectionMatrix());
  defaultShader.setMat4("view", camera.getViewMatrix());
  defaultShader.setVec3("light.direction", lightDir);
  defaultShader.setVec3("light.ambient", lightColor);
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
  auto chunkX = x / CHUNK_WIDTH;
  auto chunkY = y / CHUNK_HEIGHT;
  auto chunkZ = z / CHUNK_LENGTH;

  for (auto& chunk : chunks) {
    if (chunk->getX() == chunkX && chunk->getY() == chunkY &&
        chunk->getZ() == chunkZ) {
      auto tileX = x % CHUNK_WIDTH;
      auto tileZ = z % CHUNK_LENGTH;
      auto tileY = y % CHUNK_HEIGHT;

      return chunk->get(tileX, tileY, tileZ);
    }
  }

  return emptyTile;  // Return an empty tile if not found
}