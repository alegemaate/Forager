#include "tile.h"

// Constructor
tile::tile(int newX, int newY, int newZ, int newType, BITMAP *defaultImage){
  x = newX;
  y = newY;
  z = newZ;
  type = newType;
  biome = 0;
  temperature = 1000;

  image[0] = defaultImage;
  image[1] = defaultImage;

  name = "";
  biome_name = "";
}

// Deconstructor
tile::~tile(){

}

// Draw tile
void tile::draw( BITMAP *tempBuffer, int newTick, int zoom, int offsetX, int offsetY, BITMAP *overlayImage){
  if( type != TILE_AIR || overlayImage != NULL){
    // Temp xyz that calc zoom
    int z_x = (x * 64) /zoom;
    int z_y = (y * 64) /zoom;
    int z_z = (z * 64) /zoom;

    // Draw that image if it is onscreen
    if(image[newTick] != NULL){
      if( ((z_x   + z_z  ) + offsetX/zoom) >= -120 &&
          ((z_x   + z_z  ) + offsetX/zoom) <= SCREEN_W &&
          ((z_x/2 - z_z/2) + offsetY/zoom - z_y) >= -120 &&
          ((z_x/2 - z_z/2) + offsetY/zoom - z_y) <= SCREEN_H){
        stretch_sprite( tempBuffer, image[newTick], (z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom, 128/zoom, 128/zoom);

       /*for( int j = 0; j < 54; j ++){
          line( tempBuffer, (z_x + z_z) + offsetX, (z_x - z_z)/2 - z_y + offsetY + 32/zoom + j, (z_x + z_z) + offsetX + 64/zoom, (z_x - z_z)/2 - z_y + offsetY + j, makecol( 50, 50, 50));
          line( tempBuffer, (z_x + z_z) + offsetX + 64/zoom, (z_x - z_z)/2 - z_y + offsetY + j, (z_x + z_z) + offsetX + 128/zoom, (z_x - z_z)/2 - z_y + offsetY + 32/zoom + j, makecol( 50, 50, 50));
        }*/
        // Overlay if needed
        if(overlayImage != NULL){
          stretch_sprite( tempBuffer, overlayImage, (z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom, 128/zoom, 128/zoom);
        }
      }
      //textprintf_ex( tempBuffer,font,(z * 128 / 2) + (x * 128 / 2) + offsetX, (x * 64 / 2) - (z * 64 / 2) + offsetY - (y * 60),makecol(0,0,0),makecol(255,255,255),"%i %i %i", x, z, y);
      //textprintf_ex( tempBuffer,font,(z * 128 / 2) + (x * 128 / 2) + offsetX, (x * 64 / 2) - (z * 64 / 2) + offsetY - (y * 60),makecol(0,0,0),makecol(255,255,255),"%i", biome);
    }
  }
}

// Set biome
void tile::setBiome( int newBiome, string newBiomeName){
  biome = newBiome;
  biome_name = newBiomeName;
}

int tile::getBiome(){
  return biome;
}

// Set images
void tile::setImages(BITMAP* image1, BITMAP* image2){
  image[0] = image1;
  image[1] = image2;
}
