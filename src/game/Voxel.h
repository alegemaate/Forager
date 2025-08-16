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
  Voxel();
  explicit Voxel(TileType* type);

  unsigned short getType() { return tileImpl->getType(); }
  void setType(TileType* type) { tileImpl = type; }

  TileType* getTile() { return tileImpl; }

 private:
  TileType* tileImpl = nullptr;
};

#endif  // TILE_H
