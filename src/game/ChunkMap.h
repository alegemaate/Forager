/*
  Tile Map
  Allan Legemaate
  11/11/15
  Manages all the tiles
*/

#pragma once

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

  void generateMap();

  void render();

  Voxel& getTile(unsigned int x, unsigned int y, unsigned int z);

 private:
  // All chunks
  std::vector<std::unique_ptr<Chunk>> chunks;

  void quickPeek();

  BiomeManager biomes;
};
