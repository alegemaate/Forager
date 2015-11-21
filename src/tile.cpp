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
      stretch_sprite( tempBuffer, image[newTick], (z_x + z_z) + offsetX/zoom - (image[newTick] -> w / 2 - 64)/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom - (image[newTick] -> h - 128)/zoom, image[newTick]->w/zoom, image[newTick]->h/zoom);

      // Overlay if needed
      if(overlayImage != NULL){
        stretch_sprite( tempBuffer, overlayImage, (z_x + z_z) + offsetX/zoom - (image[newTick] -> w / 2 - 64)/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom- (image[newTick] -> h - 128)/zoom, image[newTick]->w/zoom, image[newTick]->h/zoom);
      }

      //line( tempBuffer, (z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom, (z_x + z_z) + (offsetX + 64)/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom, makecol( 50, 50, 50));
      //line( tempBuffer, (z_x + z_z) + (offsetX + 64)/zoom, (z_x - z_z)/2 - z_y + offsetY/zoom, (z_x + z_z) + (offsetX + 128)/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom, makecol( 50, 50, 50));

      if( key[KEY_T])textprintf_ex( tempBuffer,font,(z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom,makecol(0,0,0),makecol(255,255,255),"%i %i %i", x, z, y);
      //textprintf_ex( tempBuffer,font,(z_x + z_z) + offsetX/zoom, (z_x - z_z)/2 - z_y + (offsetY + 32)/zoom,makecol(0,0,0),makecol(255,255,255),"%i", biome);
    }
  }
}

// Checkif its onscreen
bool tile::onScreen( int zoom, int offsetX, int offsetY){
  // Temp xyz that calc zoom
  int z_x = (x * 64) /zoom;
  int z_y = (y * 64) /zoom;
  int z_z = (z * 64) /zoom;

  if(((z_x   + z_z  ) + offsetX/zoom) >= -120 &&
     ((z_x   + z_z  ) + offsetX/zoom) <= SCREEN_W &&
     ((z_x/2 - z_z/2) + offsetY/zoom - z_y) >= -120 &&
     ((z_x/2 - z_z/2) + offsetY/zoom - z_y) <= SCREEN_H)
      return true;

  return false;
}

// Set biome
void tile::setBiome( int newBiome, std::string newBiomeName){
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
