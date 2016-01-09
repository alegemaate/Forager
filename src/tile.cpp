#include "tile.h"

// Constructor
tile::tile(unsigned char newX, unsigned char newY, unsigned char newZ, tile_type *newType){
  x = newX;
  y = newY;
  z = newZ;

  xJiggle = 0;
  yJiggle = 0;
  zJiggle = 0;

  biome = 0;

  setType( newType);
}

// Deconstructor
tile::~tile(){

}

// Jiggle them tiles
void tile::jiggle( char newxJiggle, char newyJiggle, char newzJiggle){
  xJiggle = newxJiggle;
  yJiggle = newyJiggle;
  zJiggle = newzJiggle;
}

// Set tile type
void tile::setType(tile_type *val) {
  tileImpl = val;
}

// Draw tile
void tile::draw( int newTick){
  tileImpl -> draw( x +  double(xJiggle)/100, y + double(yJiggle)/100, z + double(zJiggle)/100, newTick);
}
