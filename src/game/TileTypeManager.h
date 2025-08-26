/*
  Tile Type Manager
  Allan Legemaate
  24/11/15
  This loads all the types of tiles into a container for access by tile objects.
*/

#ifndef TILE_TYPE_MANAGER_H
#define TILE_TYPE_MANAGER_H

#include <array>
#include <string>

#include "TileType.h"

class TileTypeManager
{
  public:
    void      load(const std::string& path);
    TileType* getTileByType(TileID tileID);

  private:
    std::array<TileType, static_cast<size_t>(TileID::Max)> tileTypes;
};

#endif // TILE_TYPE_MANAGER_H
