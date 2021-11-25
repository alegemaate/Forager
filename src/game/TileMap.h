/*
  Tile Map
  Allan Legemaate
  11/11/15
  Manages all the tiles
*/

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <allegro.h>
#include <string>

#include "BiomeManager.h"
#include "Chunk.h"
#include "Skybox.h"
#include "Tile.h"
#include "TileTypeManager.h"

class TileMap {
 public:
  explicit TileMap(BITMAP* tempBuffer);

  void update();
  void generateMap();
  void draw();

  Tile* getTile(unsigned int x, unsigned int y, unsigned int z);

 private:
  // All chunks
  Chunk* allChunks[WORLD_LENGTH][WORLD_WIDTH]{};

  void quickPeek(const std::string& currentPhase);

  BITMAP* buffPoint;

  BiomeManager biomes;
};

#endif  // TILE_MAP_H
