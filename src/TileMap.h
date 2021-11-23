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

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "BiomeManager.h"
#include "TileTypeManager.h"
#include "chunk.h"
#include "skybox.h"
#include "tile.h"

typedef glm::tvec3<GLbyte> byte3;

class TileMap {
 public:
  TileMap(BITMAP* tempBuffer);

  void load_images();
  void update();
  void generateMap();
  void draw(int newAnimationFrame);

  tile* map_tiles[DEFAULT_MAP_WIDTH][DEFAULT_MAP_LENGTH][DEFAULT_MAP_HEIGHT];

  TileTypeManager* getManager() { return &all_tile_defs; }

  skybox theSky;

 private:
  // All chunks
  chunk* allChunks[WORLD_LENGTH][WORLD_HEIGHT][WORLD_WIDTH];

  void quickPeek(const std::string& currentPhase);
  long checkBiomeNumber(char biomeToCheck);

  BITMAP* overlay_images[20];
  BITMAP* buffPoint;

  TileTypeManager all_tile_defs;
  BiomeManager biomes;
};

#endif  // TILE_MAP_H
