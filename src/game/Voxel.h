/*
  Tile
  Allan Legemaate
  11/11/15
  Class for the tile data (for images see TextureLoader.h)
*/

#ifndef TILE_H
#define TILE_H

#include "TileType.h"
#include "glm/glm.hpp"

class Voxel {
 public:
  Voxel(glm::u8vec3 position, TileType* type);

  unsigned short getType() { return tileImpl->getType(); }
  TileType* getTile() { return tileImpl; }

  void setType(TileType* val);

  // Get X value
  glm::u8vec3 getPosition() const { return position; }

  // Set/Get biome
  void setBiome(int id) { biome = id; }
  unsigned char getBiome() const { return biome; }

 private:
  glm::u8vec3 position;

  unsigned char biome = 0;

  TileType* tileImpl = nullptr;
};

#endif  // TILE_H
