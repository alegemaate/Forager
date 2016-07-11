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
    tile(unsigned char newX, unsigned char newY, unsigned char newZ, tile_type *newType);
    virtual ~tile();

    unsigned short getType() { return tileImpl -> getType(); }
    tile_type * getTile() { return tileImpl; }
    void setType(tile_type *val);

    void draw( int newTick);

    // Get X value
    unsigned char getX() { return x; }
    unsigned char getY() { return y; }
    unsigned char getZ() { return z; }

    void setX( unsigned char newVal) { x = newVal; }
    void setY( unsigned char newVal) { y = newVal; }
    void setZ( unsigned char newVal) { z = newVal; }

    void jiggle( char newxJiggle, char newyJiggle, char newzJiggle);

    // Set/Get biome
    void setBiome( int newBiomeID){ biome = newBiomeID; }
    unsigned char getBiome(){ return biome;}
  protected:
  private:
    unsigned char x;
    unsigned char y;
    unsigned char z;
    char xJiggle;
    char yJiggle;
    char zJiggle;

    unsigned char biome;

    tile_type *tileImpl;
};

#endif // TILE_H
