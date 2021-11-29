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
#include "Tile.h"

class TileMap {
 public:
  explicit TileMap();

  void update();
  void generateMap(BITMAP* buffer);
  void draw();

  Tile* getTile(unsigned int x, unsigned int y, unsigned int z);

 private:
  // All chunks
  Chunk* allChunks[WORLD_WIDTH][WORLD_LENGTH]{};

  void quickPeek(BITMAP* buffer, const std::string& currentPhase);

  BiomeManager biomes;
};

#endif  // TILE_MAP_H
