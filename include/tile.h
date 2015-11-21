/*
  Tile
  Allan Legemaate
  11/11/15
  Class for the tile data (for images see image_manager.h)
*/

#ifndef TILE_H
#define TILE_H

#include <allegro.h>
#include <string>

#include "ids.h"

class tile{
  public:
    tile(int newX, int newY, int newZ, int newType, BITMAP *defaultImage);
    virtual ~tile();

    int getType() { return type; }
    void setType(int val, std::string val2) { type = val; name = val2;}

    std::string getName() { return name;}
    std::string getBiomeName() { return biome_name;}

    int getX() { return x; }
    int getY() { return y; }
    int getZ() { return z; }

    bool onScreen( int zoom, int offsetX, int offsetY);

    int getTemperature() { return temperature; }
    void setTemperature(int val) { temperature = val; }

    int getBiome();
    void setBiome( int newBiome, std::string newBiomeName);

    void draw( BITMAP *tempBuffer, int newTick, int zoom, int offsetX, int offsetY, BITMAP *overlayImage);
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
    int temperature;

    std::string name;
    std::string biome_name;
};

#endif // TILE_H
