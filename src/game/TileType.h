/*
  Tile Type
  Allan Legemaate
  24/11/15
  Definitions of the tiles in game
*/

#pragma once

#include <array>
#include <stdexcept>
#include <string>

// Tiles
enum class TileID {
  Air = 0,
  Grass = 1,
  Sand = 2,
  Snow = 3,
  Stone = 4,
  Tree = 5,
  Rock = 6,
  Water = 7,
  Ice = 8,
  Cactus = 9,
  Lava = 10,
  Tallgrass = 11,
  GrassSnow = 12,
  TreePine = 13,
  Temp = 14,    // Temporary tile for testing
  Johnny = 15,  // Special tile for Johnny
  Dirt = 16
};

inline TileID intToTileID(int id) {
  if (id < static_cast<int>(TileID::Air) ||
      id > static_cast<int>(TileID::Dirt)) {
    throw std::runtime_error("Invalid TileID: " + std::to_string(id));
  }
  return static_cast<TileID>(id);
}

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
