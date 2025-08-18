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

#include "./BiomeManager.h"
#include "./Chunk.h"
#include "./TileTypeManager.h"
#include "./Voxel.h"

class World;

class ChunkMap {
 public:
  void update(World& world);

  void generate(TileTypeManager& tileManager);

  void render(World& world);

  Voxel& getTile(unsigned int x, unsigned int y, unsigned int z);

  Voxel& getTile(const glm::vec3& pos) {
    return getTile(static_cast<unsigned int>(pos.x),
                   static_cast<unsigned int>(pos.y),
                   static_cast<unsigned int>(pos.z));
  }

 private:
  // All chunks
  std::vector<std::unique_ptr<Chunk>> chunks;

  Voxel emptyTile{};
};
