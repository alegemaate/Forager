#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <allegro.h>
#include <string>

#include "tile.h"
#include "ids.h"
#include "tools.h"
#include "biome_manager.h"
#include "biome.h"

class tile_map{
  public:
    tile_map( std::string newType, BITMAP *tempBuffer);
    virtual ~tile_map();

    int Getx() { return x; }
    void Setx(int val) { x = val; }
    int Gety() { return y; }
    void Sety(int val) { y = val; }

    int x;
    int y;
    int z;

    tile *map_tiles[DEFAULT_MAP_WIDTH][DEFAULT_MAP_LENGTH][DEFAULT_MAP_HEIGHT];

    void load_images();
    void update();
    void generateMap(std::string newType);
    void draw( int newAnimationFrame);
  protected:

  private:
    int zoom;

    void quickPeek( std::string currentPhase);
    long checkBiomeNumber( int biomeToCheck);
    void refreshTileImages();

    int sel_x, sel_y, sel_z;

    BITMAP *errorTile;
    BITMAP *tile_images[100][2];
    BITMAP *overlay_images[20];
    BITMAP *buffPoint;

    biome_manager biomes;
};

#endif // TILE_MAP_H
