#include "Tile.h"

// Constructor
Tile::Tile(glm::u8vec3 position, TileType* type) : position(position) {
  setType(type);
}

// Set tile type
void Tile::setType(TileType* val) {
  tileImpl = val;
}
