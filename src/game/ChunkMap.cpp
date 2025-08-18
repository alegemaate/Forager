#include "ChunkMap.h"

#include <asw/asw.h>

#include "../core/Logger.h"
#include "../utils/utils.h"
#include "./TileTypeManager.h"
#include "./World.h"

constexpr size_t WORLD_WIDTH = 4;
constexpr size_t WORLD_LENGTH = 4;
constexpr size_t WORLD_HEIGHT = 1;

// Construct
ChunkMap::ChunkMap() {
  // Load biomes
  biomes.load("assets/data/biomes.json");

  // Load tiles
  TileTypeManager::load("assets/data/tiles.json");
}

// Update map
void ChunkMap::update(World& world) {
  for (auto& chunk : chunks) {
    chunk->update();
  }
}

// Procedural Generation of map
void ChunkMap::generate() {
  // GENERATE MAP
  Logger::heading("Generating Map");

  // Clear chunks
  chunks.clear();

  auto seed = random(0, 10000);

  int currentChunk = 0;
  const int worldSize = WORLD_WIDTH * WORLD_LENGTH * WORLD_HEIGHT;

  // Make lots of chunks
  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int t = 0; t < WORLD_HEIGHT; t++) {
      for (unsigned int j = 0; j < WORLD_LENGTH; j++) {
        // quickPeek();

        auto& chunk = chunks.emplace_back(std::make_unique<Chunk>(i, t, j));
        chunk->generate(seed);
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
  auto& defaultShader = world.getShaderManager().getShader("default");

  // Activate shader
  defaultShader.activate();
  defaultShader.setMat4("projection", world.getCamera().getProjectionMatrix());
  defaultShader.setMat4("view", world.getCamera().getViewMatrix());
  defaultShader.setVec3("light.direction", world.getLightDir());
  defaultShader.setVec3("light.ambient", world.getLightColor());
  defaultShader.setVec3("light.color", world.getLightColor());

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
      auto tileY = y % CHUNK_LENGTH;

      return chunk->get(tileX, tileY, tileZ);
    }
  }
}