#ifndef TILE_H
#define TILE_H

#include <allegro.h>

#include "ids.h"
#include "tools.h"

class tile{
  public:
    tile(int newX, int newY, int newZ, int newType, BITMAP *defaultImage);
    virtual ~tile();

    int getType() { return type; }
    void setType(int val) { type = val; }

    bool getBiomeType() { return biome_done; }
    void setBiomeType(int val) { biome_done = val; }

    int getBiome();
    void setBiome( int newBiome);

    void draw( BITMAP *tempBuffer, int newTick, int zoom, int offsetX, int offsetY);
    void logic();
    void setImages(BITMAP* image1, BITMAP* image2);

    BITMAP* image[2];
  protected:
  private:
    int x;
    int y;
    int z;
    int type;
    int biome;
    int biome_done;
};

#endif // TILE_H
