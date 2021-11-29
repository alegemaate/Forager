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
#include "Voxel.h"

class ChunkMap {
 public:
  explicit ChunkMap();

  void update();

  void generateMap(BITMAP* buffer);
  
  void draw();

  Voxel* getTile(unsigned int x, unsigned int y, unsigned int z);

 private:
  // All chunks
  Chunk* chunks[WORLD_WIDTH][WORLD_LENGTH]{};

  void quickPeek(BITMAP* buffer, const std::string& currentPhase);

  BiomeManager biomes;
};

#endif  // TILE_MAP_H
