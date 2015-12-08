#include "tile.h"

// Constructor
tile::tile(int newX, int newY, int newZ, tile_type *newType){
  x = newX * 32;
  y = newY * 32;
  z = newZ * 32;

  biome = 0;

  setType( newType);
}

// Deconstructor
tile::~tile(){

}

// Set tile type
void tile::setType(tile_type *val) {
  tileImpl = val;
}

// Draw tile
void tile::draw( BITMAP *tempBuffer, int newTick, int zoom, int offsetX, int offsetY, BITMAP *overlayImage){
  // Temp xyz that calc zoom
  if( VIEW_MODE == 1){
    tileImpl -> draw( tempBuffer, x/64, y/64, z/64, newTick, zoom, offsetX, offsetY);
  }
  else{
    tileImpl -> draw( tempBuffer, x/zoom, y/zoom, z/zoom, newTick, zoom, offsetX, offsetY);
  }

  // Overlay if needed
  if(overlayImage != NULL){
    //stretch_sprite( tempBuffer, overlayImage, (z_x + z_z) + offsetX/zoom - (image[newTick] -> w / 2 - 64)/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom- (image[newTick] -> h - 128)/zoom, image[newTick]->w/zoom, image[newTick]->h/zoom);
  }

  /*line( tempBuffer, (z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom, (z_x + z_z) + (offsetX + 64)/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom, makecol( 50, 50, 50));
  line( tempBuffer, (z_x + z_z) + (offsetX + 64)/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom, (z_x + z_z) + (offsetX + 128)/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom, makecol( 50, 50, 50));

  if( key[KEY_T])textprintf_ex( tempBuffer,font,(z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom,makecol(0,0,0),makecol(255,255,255),"%i %i %i", x, z, y);
  textprintf_ex( tempBuffer,font,(z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom,makecol(0,0,0),makecol(255,255,255),"%i", biome);
  */
}
