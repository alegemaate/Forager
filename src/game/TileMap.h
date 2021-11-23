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

#include "BiomeManager.h"
#include "Chunk.h"
#include "Skybox.h"
#include "Tile.h"
#include "TileTypeManager.h"

class TileMap {
 public:
  explicit TileMap(BITMAP* tempBuffer);

  void update();
  void generateMap();
  void draw(int newAnimationFrame);

  Tile* map_tiles[DEFAULT_MAP_WIDTH][DEFAULT_MAP_LENGTH][DEFAULT_MAP_HEIGHT];

  TileTypeManager* getManager() { return &all_tile_defs; }

  Skybox theSky;

 private:
  // All chunks
  Chunk* allChunks[WORLD_LENGTH][WORLD_HEIGHT][WORLD_WIDTH];

  void quickPeek(const std::string& currentPhase);
  long checkBiomeNumber(char biomeToCheck);

  BITMAP* buffPoint;

  TileTypeManager all_tile_defs;
  BiomeManager biomes;
};

#endif  // TILE_MAP_H
