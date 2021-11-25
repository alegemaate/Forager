#include "TileMap.h"

#include <iostream>

#include "../utils/utils.h"

// Construct
TileMap::TileMap(BITMAP* tempBuffer) {
  // Start in view mode
  gameMode = false;

  // Buffer
  buffPoint = tempBuffer;

  // Load biomes
  biomes.load("assets/data/biomes.xml");

  // Load tiles
  TileTypeManager::load("assets/data/tiles.xml");

  // Make lots of chunk
  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int t = 0; t < WORLD_LENGTH; t++) {
      if (!allChunks[i][t]) {
        allChunks[i][t] = new Chunk(i, t);
      }
    }
  }
}

// Update map
void TileMap::update() {
  // Gen
  if (key[KEY_R]) {
    generateMap();
  }
}

// Procedural Generation of map
void TileMap::generateMap() {
  // GENERATE MAP
  std::cout << "GENERATING MAP\n--------------\n";

  auto seed = random(0, 10000);

  for (unsigned int i = 0; i < WORLD_WIDTH; i++) {
    for (unsigned int t = 0; t < WORLD_LENGTH; t++) {
      quickPeek("Generating Chunk " + std::to_string(i * WORLD_WIDTH + t + 1) +
                "/" + std::to_string(WORLD_WIDTH * WORLD_LENGTH));

      allChunks[i][t]->generate(seed);
    }
  }
}

// Quick Peek
void TileMap::quickPeek(const std::string& currentPhase) {
  // Send to console
  std::cout << currentPhase << std::endl;

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
  rectfill(buffPoint, 0, 0, SCREEN_W, SCREEN_H, makecol(255, 0, 255));

  // Info
  textprintf_centre_ex(buffPoint, font, SCREEN_W / 2, SCREEN_H / 2,
                       makecol(0, 0, 0), makecol(255, 255, 255), "%s",
                       currentPhase.c_str());

  // Draw to screen
  draw_sprite(screen, buffPoint, 0, 0);

  allegro_gl_unset_allegro_mode();
  allegro_gl_flip();
}

// Draw map
void TileMap::draw() {
  for (auto& allChunk : allChunks) {
    for (auto& t : allChunk) {
      if (t) {
        t->render();
      }
    }
  }
}

Tile* TileMap::getTile(unsigned int x, unsigned int y, unsigned int z) {
  auto chunkX = x / CHUNK_WIDTH;
  auto chunkZ = z / CHUNK_LENGTH;

  if (allChunks[chunkX][chunkZ]) {
    auto tileX = x % CHUNK_WIDTH;
    auto tileZ = z % CHUNK_LENGTH;

    return allChunks[chunkX][chunkZ]->get(tileX, y, tileZ);
  }

  return nullptr;
}