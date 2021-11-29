#include "ChunkMap.h"

#include <alleggl.h>

#include "../core/Logger.h"
#include "../utils/utils.h"
#include "TileTypeManager.h"

// Construct
ChunkMap::ChunkMap() {
  // Load biomes
  biomes.load("assets/data/biomes.json");

  // Load tiles
  TileTypeManager::load("assets/data/tiles.json");

  // Make lots of chunk
  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int t = 0; t < WORLD_LENGTH; t++) {
      if (!chunks[i][t]) {
        chunks[i][t] = new Chunk(i, t);
      }
    }
  }
}

// Update map
void ChunkMap::update() {
  for (auto& chunk : chunks) {
    for (auto& t : chunk) {
      if (t) {
        t->update();
      }
    }
  }
}

// Procedural Generation of map
void ChunkMap::generateMap(BITMAP* buffer) {
  // GENERATE MAP
  Logger::heading("GENERATING MAP");

  auto seed = random(0, 10000);

  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int t = 0; t < WORLD_LENGTH; t++) {
      quickPeek(buffer, "Generating Chunk " +
                            std::to_string(i * WORLD_LENGTH + t + 1) + "/" +
                            std::to_string(WORLD_WIDTH * WORLD_LENGTH));

      chunks[i][t]->generate(seed);
    }
  }
}

// Quick Peek
void ChunkMap::quickPeek(BITMAP* buffer, const std::string& currentPhase) {
  // Send to console
  Logger::point(currentPhase);

  // View matrix
  glMatrixMode(GL_MODELVIEW);

  // Clear screen
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw tiles
  draw();

  // Allegro drawing
  glUseProgram(0);
  allegro_gl_set_allegro_mode();

  // Transparent buffer
  rectfill(buffer, 0, 0, SCREEN_W, SCREEN_H, makecol(255, 0, 255));

  // Info
  textprintf_centre_ex(buffer, font, SCREEN_W / 2, SCREEN_H / 2,
                       makecol(0, 0, 0), makecol(255, 255, 255), "%s",
                       currentPhase.c_str());

  // Draw to screen
  draw_sprite(screen, buffer, 0, 0);

  allegro_gl_unset_allegro_mode();
  allegro_gl_flip();
}

// Draw map
void ChunkMap::draw() {
  defaultShader->activate();

  // Pass projection matrix to shader
  glm::mat4 projection =
      glm::perspective(glm::radians(camera->zoom),
                       (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);
  defaultShader->setMat4("projection", projection);

  // Pass camera/view transformation
  glm::mat4 view = camera->getViewMatrix();
  defaultShader->setMat4("view", view);

  // Light position
  defaultShader->setVec3("light.direction", lightDir);
  defaultShader->setVec3("light.ambient", lightColor);
  defaultShader->setFloat("skyTime", skyTime);

  // Camera position
  defaultShader->setVec3("cameraPos", camera->position);

  // Cubemap
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 1);

  for (auto& allChunk : chunks) {
    for (auto& t : allChunk) {
      if (t) {
        t->render();
      }
    }
  }

  GpuProgram::deactivate();
}

Voxel* ChunkMap::getTile(unsigned int x, unsigned int y, unsigned int z) {
  auto chunkX = x / CHUNK_WIDTH;
  auto chunkZ = z / CHUNK_LENGTH;

  if (chunks[chunkX][chunkZ]) {
    auto tileX = x % CHUNK_WIDTH;
    auto tileZ = z % CHUNK_LENGTH;

    return chunks[chunkX][chunkZ]->get(tileX, y, tileZ);
  }

  return nullptr;
}