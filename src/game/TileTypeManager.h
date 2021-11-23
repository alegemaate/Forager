/*
  Tile Type Manager
  Allan Legemaate
  24/11/15
  This loads all the types of tiles into a container for access by tile objects.
*/

#ifndef TILE_TYPE_MANAGER_H
#define TILE_TYPE_MANAGER_H

#include <string>
#include <vector>

#include "TileType.h"

class TileTypeManager {
 public:
  void load(const std::string& newFile);
  TileType* getTileByType(int tileID);

 private:
  std::vector<TileType> tile_defs;
};

#endif  // TILE_TYPE_MANAGER_H
