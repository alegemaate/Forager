/*
  Tile Map
  Allan Legemaate
  11/11/15
  Manages all the tiles
*/

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <allegro.h>
#include <string>
#include <math.h>

#include "tile.h"
#include "ids.h"
#include "tools.h"
#include "biome_manager.h"
#include "biome.h"
#include "tile_type_manager.h"
#include "skybox.h"
#include "globals.h"

class tile_map{
  public:
    tile_map( BITMAP *tempBuffer);
    virtual ~tile_map();

    bool onScreen( int tile_x, int tile_y, int tile_z);

    void load_images();
    void update();
    void generateMap();
    void draw( int newAnimationFrame);

    tile *map_tiles[DEFAULT_MAP_WIDTH][DEFAULT_MAP_LENGTH][DEFAULT_MAP_HEIGHT];
  protected:

  private:
    int zoom;

    void quickPeek( std::string currentPhase);
    long checkBiomeNumber( char biomeToCheck);

    skybox theSky;

    BITMAP *overlay_images[20];
    BITMAP *buffPoint;

    tile_type_manager tile_defs;
    biome_manager biomes;
};

#endif // TILE_MAP_H
