#include "TileMap.h"

#include <iostream>

#include "../core/SimplexNoise.h"
#include "../utils/utils.h"

// Construct
TileMap::TileMap(BITMAP* tempBuffer) {
  // Start in view mode
  gameMode = false;

  // Load sky
  theSky.loadSkybox("images/skybox/", "front.png", "back.png", "left.png",
                    "right.png", "top.png", "bottom.png");

  // Buffer
  buffPoint = tempBuffer;

  // Load biomes
  biomes.load("data/biomes.xml");

  // Load tiles
  all_tile_defs.load("data/tiles.xml");



  // Make a map full of tiles
  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < DEFAULT_MAP_HEIGHT; u++) {
        map_tiles[i][t][u] = new Tile(i, u, t, all_tile_defs.getTileByType(0));
      }
    }
  }

  // Make lots of chunk
  for (int i = 0; i < WORLD_WIDTH; i++) {
    for (int t = 0; t < WORLD_LENGTH; t++) {
      for (int n = 0; n < WORLD_HEIGHT; n++) {
        if (!allChunks[i][n][t]) {
          allChunks[i][n][t] = new Chunk(i, n, t);
        }
      }
    }
  }
}

// Update map
void TileMap::update() {
  // Gen
  if (key[KEY_R]) {
    generateMap();
  }
}

// Procedural Generation of map
void TileMap::generateMap() {
  // GENERATE MAP
  std::cout << "GENERATING MAP\n--------------\n";

  // STEP 1:
  // Start with air
  quickPeek("Filling with Air");

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < DEFAULT_MAP_HEIGHT; u++) {
        map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(TILE_AIR));
        map_tiles[i][t][u]->setBiome(BIOME_NONE);
        map_tiles[i][t][u]->jiggle(0, 0, 0);
      }
    }
  }

  // STEP 2:
  // Fill with dirt
  quickPeek("Placing dirt");

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < 2; u++) {
        map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(TILE_GRASS));
      }
    }
  }

  // STEP 3:
  // Random Biomes
  quickPeek("Placing Random Biome Spawns");

  // Biome spawn tiles
  for (int i = 0; i < ((DEFAULT_MAP_WIDTH * DEFAULT_MAP_LENGTH) / 5000) + 1;
       i++) {
    for (int k = 0; k < biomes.getNumberBiomes(); k++) {
      if (random(0, biomes.getBiome(k).getChance()) == 1) {
        map_tiles[random(0, DEFAULT_MAP_WIDTH - 1)]
                 [random(0, DEFAULT_MAP_LENGTH - 1)][0]
                     ->setBiome(biomes.getBiome(k).getID());
      }
    }
  }

  // STEP 3b:
  // Place biome if there is none!
  if (checkBiomeNumber(BIOME_NONE) == DEFAULT_MAP_LENGTH * DEFAULT_MAP_WIDTH)
    map_tiles[0][0][0]->setBiome(biomes.getBiome(1).getID());

  // Quick Peek
  quickPeek("Filling Biomes");

  // Spread those biomes
  while (checkBiomeNumber(BIOME_NONE) > 0) {
    for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
      for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
        if (map_tiles[i][t][0]->getBiome() != BIOME_NONE) {
          switch (random(0, 3)) {
            case 0:
              if (i > 0 && map_tiles[i - 1][t][0]->getBiome() == BIOME_NONE)
                map_tiles[i - 1][t][0]->setBiome(
                    map_tiles[i][t][0]->getBiome());
              break;
            case 1:
              if (t > 0 && map_tiles[i][t - 1][0]->getBiome() == BIOME_NONE)
                map_tiles[i][t - 1][0]->setBiome(
                    map_tiles[i][t][0]->getBiome());
              break;
            case 2:
              if (i < DEFAULT_MAP_WIDTH - 1 &&
                  map_tiles[i + 1][t][0]->getBiome() == BIOME_NONE)
                map_tiles[i + 1][t][0]->setBiome(
                    map_tiles[i][t][0]->getBiome());
              break;
            case 3:
              if (t < DEFAULT_MAP_LENGTH - 1 &&
                  map_tiles[i][t + 1][0]->getBiome() == BIOME_NONE)
                map_tiles[i][t + 1][0]->setBiome(
                    map_tiles[i][t][0]->getBiome());
              break;
            default:
              break;
          }
        }
      }
    }
  }

  // Quick Peek
  quickPeek("Raising biomes");

  // Extend through all tiles
  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < DEFAULT_MAP_HEIGHT; u++) {
        map_tiles[i][t][u]->setBiome(map_tiles[i][t][0]->getBiome());
      }
    }
  }

  // STEP 5:
  // Make Rivers
  quickPeek("Carving rivers");

  // Rivers
  int numberRivers = DEFAULT_MAP_WIDTH / 20;

  for (int r = 0; r < numberRivers; r++) {
    // Rivers Remaining
    std::cout << "Rivers Remaining:" << numberRivers - r << "\n";

    // Temp Variables
    int river_x = -1;
    int river_y = -1;
    int river_end_x = -1;
    int river_end_y = -1;

    bool pathFound = false;

    int random_x;
    int random_y;

    // Find current rivers and make path
    while (!pathFound && checkBiomeNumber(BIOME_LAKE) > 0) {
      random_x = random(0, DEFAULT_MAP_WIDTH - 1);
      random_y = random(0, DEFAULT_MAP_LENGTH - 1);

      if (map_tiles[random_x][random_y][1]->getBiome() == BIOME_LAKE) {
        if (river_x == -1) {
          river_x = random_x;
          river_y = random_y;
        } else {
          river_end_x = random_x;
          river_end_y = random_y;
          pathFound = true;
        }
      }
    }

    // Carve river
    while (river_x != river_end_x || river_y != river_end_y) {
      if (river_y < DEFAULT_MAP_LENGTH && river_x < DEFAULT_MAP_WIDTH &&
          river_x >= 0 && river_y >= 0) {
        map_tiles[river_x][river_y][1]->setType(
            all_tile_defs.getTileByType(TILE_TEMP));
      }

      // Shift river
      if (random(1, 10) == 1 || (random(1, 2) == 1 && river_x < river_end_x)) {
        river_x += 1;
      }
      if (random(1, 10) == 1 || (random(1, 2) == 1 && river_x > river_end_x)) {
        river_x -= 1;
      }
      if (random(1, 10) == 1 || (random(1, 2) == 1 && river_y < river_end_y)) {
        river_y += 1;
      }
      if (random(1, 5) == 1 || (random(1, 2) == 1 && river_y > river_end_y)) {
        river_y -= 1;
      }
    }

    // Widen river
    int river_width = random(1, 3);

    for (int q = 0; q < river_width; q++) {
      int wideningDir = random(0, 1);
      for (int i = 1; i < DEFAULT_MAP_WIDTH; i++) {
        for (int t = 1; t < DEFAULT_MAP_LENGTH; t++) {
          if (map_tiles[i][t][1]->getType() == TILE_TEMP) {
            if (wideningDir == 1) {
              map_tiles[i - 1][t][1]->setType(
                  all_tile_defs.getTileByType(TILE_TEMP));
            } else {
              map_tiles[i][t - 1][1]->setType(
                  all_tile_defs.getTileByType(TILE_TEMP));
            }
          }
        }
      }
    }

    // Temp water to water
    for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
      for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
        if (map_tiles[i][t][1]->getType() == TILE_TEMP) {
          map_tiles[i][t][1]->setType(all_tile_defs.getTileByType(TILE_WATER));
        }
      }
    }
  }

  // STEP 6:
  // Raise land
  quickPeek("Creating height");

  int mountain_frequency;
  int mountain_height;
  int mountain_radius;
  int mountain_steepness;

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 2; u < DEFAULT_MAP_HEIGHT; u++) {
        // Spawning mountains
        mountain_frequency = biomes.getBiome(map_tiles[i][t][u - 1]->getBiome())
                                 .getMountainFrequency();
        mountain_height = biomes.getBiome(map_tiles[i][t][u - 1]->getBiome())
                              .getMountainHeight();
        mountain_radius = biomes.getBiome(map_tiles[i][t][u - 1]->getBiome())
                              .getMountainRadius();
        mountain_steepness = biomes.getBiome(map_tiles[i][t][u - 1]->getBiome())
                                 .getMountainSteepness();

        // Make those mountains
        if (random(0, mountain_frequency) == 1 &&
            map_tiles[i][t][u - 1]->getType() == TILE_GRASS) {
          int mountainRadius = random(1, mountain_radius);
          int mountainHeight = random(1, mountain_height);

          for (int w = 0; w < mountainHeight; w++) {
            if (u + w < DEFAULT_MAP_HEIGHT) {
              for (int q = -mountainRadius; q < mountainRadius; q++) {
                for (int r = -mountainRadius; r < mountainRadius; r++) {
                  if (i + q < DEFAULT_MAP_WIDTH && i + q >= 0 &&
                      t + r < DEFAULT_MAP_LENGTH && t + r >= 0) {
                    // Round mountains!
                    if (distanceTo2D(i, t, i + q, t + r) <
                        (mountainRadius -
                         ceil(pow(double(mountain_steepness) / 10, w)))) {
                      map_tiles[i + q][t + r][u + w]->setType(
                          all_tile_defs.getTileByType(TILE_GRASS));
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  // STEP 7:
  // Fill in holes
  quickPeek("Filling holes");

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = DEFAULT_MAP_HEIGHT - 1; u > 1; u--) {
        if (map_tiles[i][t][u]->getType() == TILE_GRASS &&
            map_tiles[i][t][u - 1]->getType() == TILE_AIR) {
          map_tiles[i][t][u - 1]->setType(
              all_tile_defs.getTileByType(TILE_GRASS));
        }
      }
    }
  }

  // Step 8a:
  // Change lakes to water
  quickPeek("Adding water to lakes");

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < DEFAULT_MAP_HEIGHT; u++) {
        if (map_tiles[i][t][u]->getType() == TILE_GRASS) {
          if (map_tiles[i][t][u]->getBiome() == BIOME_LAKE)
            map_tiles[i][t][u]->setType(
                all_tile_defs.getTileByType(TILE_WATER));
        }
      }
    }
  }

  // STEP 8b:
  // Removing "Water Mountains"
  quickPeek("Carving mountains");

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      if (map_tiles[i][t][1]->getType() == TILE_WATER) {
        for (int u = 2; u < DEFAULT_MAP_HEIGHT; u++) {
          map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(TILE_AIR));
        }
      }
    }
  }

  // STEP 9a:
  // Slating "Slating from Under"
  quickPeek("Slating Stuff from Under");

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < DEFAULT_MAP_HEIGHT; u++) {
        bool slatedForRemoval = true;
        if (i > 0 && map_tiles[i - 1][t][u]->getType() == TILE_AIR)
          slatedForRemoval = false;
        else if (t > 0 && map_tiles[i][t - 1][u]->getType() == TILE_AIR)
          slatedForRemoval = false;
        else if (u > 0 && map_tiles[i][t][u - 1]->getType() == TILE_AIR)
          slatedForRemoval = false;
        else if (i < DEFAULT_MAP_WIDTH - 1 &&
                 map_tiles[i + 1][t][u]->getType() == TILE_AIR)
          slatedForRemoval = false;
        else if (t < DEFAULT_MAP_LENGTH - 1 &&
                 map_tiles[i][t + 1][u]->getType() == TILE_AIR)
          slatedForRemoval = false;
        else if (u < DEFAULT_MAP_HEIGHT - 1 &&
                 map_tiles[i][t][u + 1]->getType() == TILE_AIR)
          slatedForRemoval = false;

        if (slatedForRemoval)
          map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(TILE_TEMP));
      }
    }
  }

  // STEP 9b:
  // Removing "Stuff from Under"
  quickPeek("Removing Stuff from Under");
  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < DEFAULT_MAP_HEIGHT; u++) {
        if (map_tiles[i][t][u]->getType() == TILE_TEMP)
          map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(TILE_AIR));
      }
    }
  }

  // STEP 10:
  // Create biome resources
  quickPeek("Creating biome resources");

  // Place objects
  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 1; u < DEFAULT_MAP_HEIGHT - 1; u++) {
        if (map_tiles[i][t][u - 1]->getType() == TILE_GRASS &&
            map_tiles[i][t][u]->getType() == TILE_AIR) {
          // Spawn tile
          map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(
              biomes.getBiome(map_tiles[i][t][u]->getBiome()).spawnResource()));
          map_tiles[i][t][u]->jiggle(
              random(0, (int)map_tiles[i][t][u]->getTile()->getRandomness()), 0,
              random(0, (int)map_tiles[i][t][u]->getTile()->getRandomness()));
        }
      }
    }
  }

  // STEP 11:
  // Change water depending on biome
  quickPeek("Changing water up");

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < DEFAULT_MAP_HEIGHT; u++) {
        // Water, to ice or lava or none
        if (map_tiles[i][t][u]->getType() == TILE_WATER) {
          if (map_tiles[i][t][u]->getBiome() == BIOME_TUNDRA)
            map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(TILE_ICE));
          else if (map_tiles[i][t][u]->getBiome() == BIOME_BARREN)
            map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(TILE_LAVA));
        }
      }
    }
  }

  // STEP 12:
  // Set biome tiles
  quickPeek("Setting corresponding biome images");

  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int u = 0; u < DEFAULT_MAP_HEIGHT; u++) {
        // Dirt, to sand or rock or snow
        if (map_tiles[i][t][u]->getType() == TILE_GRASS) {
          if (map_tiles[i][t][u]->getBiome() == BIOME_TUNDRA) {
            map_tiles[i][t][u]->setType(
                all_tile_defs.getTileByType(TILE_GRASS_SNOW));
          } else if (map_tiles[i][t][u]->getBiome() == BIOME_BARREN) {
            map_tiles[i][t][u]->setType(
                all_tile_defs.getTileByType(TILE_STONE));
          } else if (map_tiles[i][t][u]->getBiome() == BIOME_DESERT) {
            map_tiles[i][t][u]->setType(all_tile_defs.getTileByType(TILE_SAND));
          }
        }
      }
    }
  }

  // Done!
  quickPeek("Done!");
}

// Checks how many tiles don't have a biome
long TileMap::checkBiomeNumber(char biomeToCheck) {
  long numberOfBiome = 0;
  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      if (map_tiles[i][t][0]->getBiome() == biomeToCheck) {
        numberOfBiome++;
      }
    }
  }
  return numberOfBiome;
}

// Quick Peek
void TileMap::quickPeek(const std::string& currentPhase) {
  // Send to console
  std::cout << "PHASE:" << currentPhase.c_str() << "\n";

  // View matrix
  glMatrixMode(GL_MODELVIEW);

  // Clear screen
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw tiles
  draw(0);

  // Allegro drawing
  glUseProgram(0);
  allegro_gl_set_allegro_mode();

  // Transparent buffer
  rectfill(buffPoint, 0, 0, SCREEN_W, SCREEN_H, makecol(255, 0, 255));

  // Info
  textprintf_centre_ex(buffPoint, font, SCREEN_W / 2, SCREEN_H / 2,
                       makecol(0, 0, 0), makecol(255, 255, 255), "%s",
                       currentPhase.c_str());

  // Draw to screen
  draw_sprite(screen, buffPoint, 0, 0);

  allegro_gl_unset_allegro_mode();
  allegro_gl_flip();
}

// Draw map
void TileMap::draw(int newAnimationFrame) {
  // Skybox
  glUseProgram(skyShader);
  theSky.renderSkybox();
  glUseProgram(defaultShader);

  // Go through all tiles and draw
  for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
    for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
      for (int n = 0; n < DEFAULT_MAP_HEIGHT; n++) {
        map_tiles[i][t][n]->draw(newAnimationFrame);
      }
    }
  }

  for (int i = 0; i < WORLD_WIDTH; i++) {
    for (int t = 0; t < WORLD_LENGTH; t++) {
      for (int n = 0; n < WORLD_HEIGHT; n++) {
        if (allChunks[i][n][t]) {
          // allChunks[i][n][t]->render();
        }
      }
    }
  }
}
