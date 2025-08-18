#include "ChunkMap.h"

#include <asw/asw.h>

#include "../core/Logger.h"
#include "../utils/utils.h"
#include "TileTypeManager.h"

// Construct
ChunkMap::ChunkMap() {
  // Load biomes
  biomes.load("assets/data/biomes.json");

  // Load tiles
  TileTypeManager::load("assets/data/tiles.json");
}

// Update map
void ChunkMap::update() {
  for (auto& chunk : chunks) {
    chunk->update();
  }
}

// Procedural Generation of map
void ChunkMap::generateMap() {
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
        quickPeek();

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

// Quick Peek
void ChunkMap::quickPeek() {
  // Clear screen
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw tiles
  render();

  // Draw to screen
  SDL_GL_SwapWindow(asw::display::window);
}

// Draw map
void ChunkMap::render() {
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
    chunk->render();
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