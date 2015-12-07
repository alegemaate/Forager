/*
  Tile
  Allan Legemaate
  11/11/15
  Class for the tile data (for images see image_manager.h)
*/

#ifndef TILE_H
#define TILE_H

#include <allegro.h>

#include "ids.h"
#include "tile_type.h"

class tile{
  public:
    tile(int newX, int newY, int newZ, tile_type *newType);
    virtual ~tile();

    unsigned short getType() { return tileImpl -> getType(); }
    void setType(tile_type *val);

    void draw( BITMAP *tempBuffer, int newTick, int zoom, int offsetX, int offsetY, BITMAP *overlayImage);

    // Get X value
    unsigned short getX() { return x; }
    unsigned short getY() { return y; }
    unsigned short getZ() { return z; }

    // Set/Get biome
    void setBiome( int newBiomeID){ biome = newBiomeID; }
    unsigned char getBiome(){ return biome;}
  protected:
  private:
    unsigned short x;
    unsigned short y;
    unsigned short z;
    unsigned char biome;

    tile_type *tileImpl;
};

#endif // TILE_H
