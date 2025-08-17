/*
  Tile Type Manager
  Allan Legemaate
  24/11/15
  This loads all the types of tiles into a container for access by tile objects.
*/

#ifndef TILE_TYPE_MANAGER_H
#define TILE_TYPE_MANAGER_H

#include <string>
#include <unordered_map>

#include "TileType.h"

class TileTypeManager {
 public:
  static void load(const std::string& path);
  static TileType* getTileByType(TileID tileID);

 private:
  static std::unordered_map<TileID, TileType> tileTypes;
};

#endif  // TILE_TYPE_MANAGER_H
