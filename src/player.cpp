#include "player.h"

player::player(int newX, int newY, int newZ){
  x = newX;
  y = newY;
  x = newZ;

  image[0] = load_bitmap( "images/johnny_small.bmp", NULL);
}

player::~player(){

}

// Draw tile
void player::draw( BITMAP *tempBuffer, int zoom, int offsetX, int offsetY){
  // Temp xyz that calc zoom
  int z_x = (x * 64);
  int z_y = (y * 64);
  int z_z = (z * 64);

  // Draw that image if it is onscreen
  if(image[0] != NULL){
    stretch_sprite( tempBuffer, image[0], 0,0,1280,1280);

    //if( key[KEY_Y])textprintf_ex( tempBuffer,font,(z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom,makecol(0,0,0),makecol(255,255,255),"%i %i %i", x, z, y);
    //textprintf_ex( tempBuffer,font,(z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom,makecol(0,0,0),makecol(255,255,255),"%i", biome);
  }
}

// Move character and such
void logic(){

}
