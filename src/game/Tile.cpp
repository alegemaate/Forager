#include "Tile.h"

// Constructor
Tile::Tile(unsigned char x, unsigned char y, unsigned char z, TileType* type)
    : x(x), y(y), z(z) {
  setType(type);
}

// Jiggle them tiles
void Tile::jiggle(char newXJiggle, char newYJiggle, char newZJiggle) {
  xJiggle = newXJiggle;
  yJiggle = newYJiggle;
  zJiggle = newZJiggle;
}

// Set tile type
void Tile::setType(TileType* val) {
  tileImpl = val;
}
