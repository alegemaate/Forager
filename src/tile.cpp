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

  if( getType() == TILE_TREE)
    std::cout << newxJiggle << "\n";
}

// Set tile type
void tile::setType(tile_type *val) {
  tileImpl = val;
}

// Draw tile
void tile::draw( BITMAP *tempBuffer, int newTick, BITMAP *overlayImage){
  // Temp xyz that calc zoom
  tileImpl -> draw( tempBuffer, double(x + xJiggle/100), double(y + yJiggle/100), double(z + zJiggle/100), newTick);

  // Overlay if needed
  if(overlayImage != NULL){
    //stretch_sprite( tempBuffer, overlayImage, (z_x + z_z) + offsetX/zoom - (image[newTick] -> w / 2 - 64)/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom- (image[newTick] -> h - 128)/zoom, image[newTick]->w/zoom, image[newTick]->h/zoom);
  }

  /*
  if( key[KEY_T])textprintf_ex( tempBuffer,font,(z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom,makecol(0,0,0),makecol(255,255,255),"%i %i %i", x, z, y);
  textprintf_ex( tempBuffer,font,(z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom,makecol(0,0,0),makecol(255,255,255),"%i", biome);
  */
}
