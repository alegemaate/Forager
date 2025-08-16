/*
  Tile Map
  Allan Legemaate
  11/11/15
  Manages all the tiles
*/

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <allegro.h>
#include <memory>
#include <string>
#include <vector>

#include "BiomeManager.h"
#include "Chunk.h"
#include "Voxel.h"

class ChunkMap {
 public:
  explicit ChunkMap();

  void update();

  void generateMap(BITMAP* buffer);

  void draw();

  Voxel& getTile(unsigned int x, unsigned int y, unsigned int z);

 private:
  // All chunks
  std::vector<std::unique_ptr<Chunk>> chunks{};

  void quickPeek(BITMAP* buffer, const std::string& currentPhase);

  BiomeManager biomes;
};

#endif  // TILE_MAP_H
