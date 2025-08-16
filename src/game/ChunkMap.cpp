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
  Logger::heading("GENERATING MAP");

  // Clear chunks
  chunks.clear();

  auto seed = random(0, 10000);

  int currentChunk = 0;
  const int worldSize = WORLD_WIDTH * WORLD_LENGTH * WORLD_HEIGHT;

  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int t = 0; t < WORLD_HEIGHT; t++) {
      for (unsigned int j = 0; j < WORLD_LENGTH; j++) {
      }
    }
  }

  // Make lots of chunks
  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int t = 0; t < WORLD_HEIGHT; t++) {
      for (unsigned int j = 0; j < WORLD_LENGTH; j++) {
        std::string currentPhase = "Generating Chunk " +
                                   std::to_string(currentChunk + 1) + "/" +
                                   std::to_string(worldSize);
        quickPeek("Generating Chunk " + std::to_string(currentChunk + 1) + "/" +
                  std::to_string(worldSize));
        auto& chunk = chunks.emplace_back(std::make_unique<Chunk>(i, t, j));
        chunk->generate(seed);
        currentChunk++;
      }
    }
  }
}

// Quick Peek
void ChunkMap::quickPeek(const std::string& currentPhase) {
  const auto screenSize = asw::display::getSize();

  // Send to console
  Logger::point(currentPhase);

  // View matrix
  glMatrixMode(GL_MODELVIEW);

  // Clear screen
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw tiles
  draw();

  // SDL drawing
  // glUseProgram(0);
  // allegro_gl_set_allegro_mode();

  // Transparent buffer
  // asw::draw::rectFill(
  //     asw::Quad<float>{0.0F, 0.0F, static_cast<float>(screenSize.x),
  //                      static_cast<float>(screenSize.y)},
  //     asw::util::makeColor(255, 0, 255));

  // Info TODO
  // textprintf_centre_ex(buffer, font, SCREEN_W / 2, SCREEN_H / 2,
  //                      makecol(0, 0, 0), makecol(255, 255, 255), "%s",
  //                      currentPhase.c_str());

  // // Draw to screen
  // draw_sprite(screen, buffer, 0, 0);

  // allegro_gl_unset_allegro_mode();
  // allegro_gl_flip();
}

// Draw map
void ChunkMap::draw() {
  auto screenSize = asw::display::getSize();

  defaultShader->activate();

  // Pass projection matrix to shader
  glm::mat4 projection =
      glm::perspective(glm::radians(camera->zoom),
                       (float)screenSize.x / (float)screenSize.y, 0.1f, 100.0f);
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

  // Cube map
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 1);

  for (auto& chunk : chunks) {
    chunk->render();
  }

  GpuProgram::deactivate();
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