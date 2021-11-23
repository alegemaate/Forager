#include "tile.h"

// Constructor
tile::tile(unsigned char x, unsigned char y, unsigned char z, TileType* type)
    : x(x), y(y), z(z) {
  setType(type);
}

// Jiggle them tiles
void tile::jiggle(char newXJiggle, char newYJiggle, char newZJiggle) {
  xJiggle = newXJiggle;
  yJiggle = newYJiggle;
  zJiggle = newZJiggle;
}

// Set tile type
void tile::setType(TileType* val) {
  tileImpl = val;
}

// Draw tile
void tile::draw(int tick) {
  tileImpl->draw(static_cast<float>(x) + static_cast<float>(xJiggle) / 100.0f,
                 static_cast<float>(y) + static_cast<float>(yJiggle) / 100.0f,
                 static_cast<float>(z) + static_cast<float>(zJiggle) / 100.0f,
                 tick);
}
