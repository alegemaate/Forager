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

#include "biome_manager.h"
#include "chunk.h"
#include "skybox.h"
#include "tile.h"
#include "tile_type_manager.h"

typedef glm::tvec3<GLbyte> byte3;

class tile_map {
 public:
  tile_map(BITMAP* tempBuffer);
  virtual ~tile_map();

  bool onScreen(int tile_x, int tile_y, int tile_z);

  void load_images();
  void update();
  void generateMap();
  void draw(int newAnimationFrame);

  tile* map_tiles[DEFAULT_MAP_WIDTH][DEFAULT_MAP_LENGTH][DEFAULT_MAP_HEIGHT];

  tile_type_manager* getManager() { return &all_tile_defs; }

  skybox theSky;

 protected:
 private:
  // All chunks
  chunk* allChunks[WORLD_LENGTH][WORLD_HEIGHT][WORLD_WIDTH];

  void quickPeek(std::string currentPhase);
  long checkBiomeNumber(char biomeToCheck);

  BITMAP* overlay_images[20];
  BITMAP* buffPoint;

  tile_type_manager all_tile_defs;
  biome_manager biomes;
};

#endif  // TILE_MAP_H
