/*
  Tile
  Allan Legemaate
  11/11/15
  Class for the tile data (for images see TextureLoader.h)
*/

#pragma once

#include <glm/glm.hpp>
#include "TileType.h"

class Voxel {
 public:
  Voxel();
  explicit Voxel(TileType* type);

  TileID getType() { return tileImpl->getType(); }
  void setType(TileType* type) { tileImpl = type; }

  TileType* getTile() { return tileImpl; }

 private:
  TileType* tileImpl = nullptr;
};
