/*
  Tile Type Manager
  Allan Legemaate
  24/11/15
  This loads all the types of tiles into a container for access by tile objects.
*/

#pragma once

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
