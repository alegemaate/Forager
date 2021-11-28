/*
  Tile Type
  Allan Legemaate
  24/11/15
  Definitions of the tiles in game
*/

#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include <array>
#include <string>

struct AtlasLookup {
  unsigned int top;
  unsigned int bottom;
  unsigned int left;
  unsigned int right;
  unsigned int front;
  unsigned int back;
};

class TileType {
 public:
  TileType(unsigned char type, AtlasLookup atlasId);

  // Get type
  unsigned char getType() const { return type; }

  // Get atlas ids
  AtlasLookup getAtlasIds() const { return atlasIds; }

 private:
  unsigned char type;
  AtlasLookup atlasIds;
};

#endif  // TILE_TYPE_H
