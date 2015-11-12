#include "tile_map.h"

// Construct
tile_map::tile_map( std::string newType, BITMAP *tempBuffer){
  // Starting position and zoom
  x = 40;
  y = 600;
  z = 0;
  zoom = 5;

  // Make a mapful of tiles
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        map_tiles[i][t][u] = new tile( i, u, t, 0, NULL);
      }
    }
  }

  // Buffer
  buffPoint = tempBuffer;

  // Randomly generate terrain
  //generateMap(newType);
}

// Deconstruct
tile_map::~tile_map(){
  //dtor
}

// Load images for tiles
void tile_map::load_images(){
  // Error
  errorTile = load_bitmap("images/tiles/NULL.png", NULL);

  // In case of error
  for( int i = 0; i < 100; i ++){
    for( int t = 0; t < 2; t ++){
      tile_images[i][t] = errorTile;
    }
  }

  // Static
  tile_images[TILE_AIR][0] = load_bitmap("images/tiles/air.bmp", NULL);
  tile_images[TILE_GRASS][0] = load_bitmap("images/tiles/grass.bmp", NULL);
  tile_images[TILE_ROCK][0] = load_bitmap("images/tiles/rock.bmp", NULL);
  tile_images[TILE_STONE][0] = load_bitmap("images/tiles/stone.png", NULL);
  tile_images[TILE_SAND][0] = load_bitmap("images/tiles/sand.png", NULL);
  tile_images[TILE_SNOW][0] = load_bitmap("images/tiles/snow.png", NULL);
  tile_images[TILE_ICE][0] = load_bitmap("images/tiles/ice.png", NULL);
  tile_images[TILE_CACTUS][0] = load_bitmap("images/tiles/cactus.png", NULL);
  tile_images[TILE_LAVA][0] = load_bitmap("images/tiles/lava.png", NULL);
  tile_images[TILE_TALLGRASS][0] = load_bitmap("images/tiles/tallgrass.png", NULL);
  tile_images[TILE_GRASS_SNOW][0] = load_bitmap("images/tiles/grass_snow.bmp", NULL);

  // Animated
  tile_images[TILE_TREE][0] = load_bitmap("images/tiles/tree1.bmp", NULL);
  tile_images[TILE_TREE][1] = load_bitmap("images/tiles/tree2.bmp", NULL);

  tile_images[TILE_WATER][0] = load_bitmap("images/tiles/water1.bmp", NULL);
  tile_images[TILE_WATER][1] = load_bitmap("images/tiles/water2.bmp", NULL);

  // Sets all the tiles images
  refreshTileImages();
}

// Update map
void tile_map::update(){
  //Up
  if(key[KEY_UP]){
    y += 5;
  }
  //Down
  if(key[KEY_DOWN]){
    y -= 5;
  }
  //Left
  if(key[KEY_LEFT]){
    x += 5;
  }
  //Right
  if(key[KEY_RIGHT]){
    x -= 5;
  }
  //Zoom in
  if(key[KEY_Q]){
    zoom ++;
    rest(40);
  }
  //Zoom out
  if(key[KEY_A]){
    if(zoom > 1){
      zoom --;
      rest(40);
    }
  }
  //Gen
  if(key[KEY_R]){
    generateMap("normal");
  }
}

// Procedural Generation of map
void tile_map::generateMap(std::string newType){
  // STEP 1:
  // Start with air
  quickPeek( "Filling with Air");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        map_tiles[i][t][u] -> setType(TILE_AIR);
        map_tiles[i][t][u] -> setBiome(BIOME_NONE);
      }
    }
  }

  // STEP 2:
  // Fill with dirt
  quickPeek( "Placing dirt");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  2; u++){
        map_tiles[i][t][u] -> setType(TILE_GRASS);
      }
    }
  }

  // STEP 3:
  // Random Biomes
  quickPeek( "Placing Random Biome Spawns");

  //Biome spawn tiles
  for( int i = 0; i < ((DEFAULT_MAP_WIDTH * DEFAULT_MAP_LENGTH)/6000) + 1; i++){
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_BARREN);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_TUNDRA);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_GRASSLAND);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_TUNDRA);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_DESERT);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_FOREST);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_LAKE);
  }

  // Quick Peek
  quickPeek( "Filling Biomes");

  // Spread thoe biomes
  while( checkBiomeless() > 0){
    for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
      for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
        if( map_tiles[i][t][0] -> getBiome() != BIOME_NONE){
          switch( random( 0, 3)){
            case 0:
              if( i > 0 && map_tiles[i - 1][t][0] -> getBiome() == BIOME_NONE)
                map_tiles[i - 1][t][0] -> setBiome( map_tiles[i][t][0] -> getBiome());
              break;
            case 1:
              if( t > 0 && map_tiles[i][t - 1][0] -> getBiome() == BIOME_NONE)
                map_tiles[i][t - 1][0] -> setBiome( map_tiles[i][t][0] -> getBiome());
              break;
            case 2:
              if( i < DEFAULT_MAP_WIDTH - 1 && map_tiles[i + 1][t][0] -> getBiome() == BIOME_NONE)
                map_tiles[i + 1][t][0] -> setBiome( map_tiles[i][t][0] -> getBiome());
              break;
            case 3:
              if( t < DEFAULT_MAP_LENGTH - 1 && map_tiles[i][t + 1][0] -> getBiome() == BIOME_NONE)
                map_tiles[i][t + 1][0] -> setBiome( map_tiles[i][t][0] -> getBiome());
              break;
            default:
              break;
          }
        }
      }
    }
    textprintf_ex( screen,font, 0, 20,makecol(0,0,0),makecol(255,255,255),"Tiles Remaining: %10d", checkBiomeless());
  }

  // Quick Peek
  quickPeek( "Raising biomes");

  // Extend through all tiles
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        map_tiles[i][t][u] -> setBiome( map_tiles[i][t][0] -> getBiome());
      }
    }
  }

  // STEP 5:
  // Make Rivers
  quickPeek( "Carving rivers");

  // Rivers
  int numberRivers = DEFAULT_MAP_WIDTH/20;

  for( int r = 0; r < numberRivers; r++){
    // Rivers Remaining
    textprintf_ex( screen,font, 0, 20,makecol(0,0,0),makecol(255,255,255),"Rivers Remaining: %10d", numberRivers-r);

    // Temp Variables
    int river_x = -1;
    int river_y = -1;
    int river_end_x = -1;
    int river_end_y = -1;

    bool pathFound = false;

    int random_x = 0;
    int random_y = 0;

    // Find current rivers and make path
    while( !pathFound){
      random_x = random( 0, DEFAULT_MAP_WIDTH - 1);
      random_y = random( 0, DEFAULT_MAP_LENGTH - 1);

      if( map_tiles[random_x][random_y][1] -> getBiome() == BIOME_LAKE){
        if( river_x == -1){
          river_x = random_x;
          river_y = random_y;
        }
        else{
          river_end_x = random_x;
          river_end_y = random_y;
          pathFound = true;
        }
      }
    }

    // Carve river
    while( river_x != river_end_x || river_y != river_end_y){
      if( river_y < DEFAULT_MAP_LENGTH && river_x < DEFAULT_MAP_WIDTH && river_x >= 0 && river_y >= 0){
        map_tiles[river_x][river_y][1] -> setType(TILE_TEMP_WATER);
      }

      // Shift river
      // Zero in on final destiantion
      if( random( 1, 10) == 1 || random( 1, 2) == 1 && river_x < river_end_x){
        river_x += 1;
      }
      if( random( 1, 10) == 1 || random( 1, 2) == 1 && river_x > river_end_x){
        river_x -= 1;
      }
      if( random( 1, 10) == 1 || random( 1, 2) == 1 && river_y < river_end_y){
        river_y += 1;
      }
      if( random( 1, 5) == 1 || random( 1, 2) == 1 && river_y > river_end_y){
        river_y -= 1;
      }
    }

    // Widen river
    int river_width = random( 1, 3);

    for( int q = 0; q < river_width; q++){
      int wideningDir = random( 0, 1);
      for(int i = 1; i < DEFAULT_MAP_WIDTH; i++){
        for(int t = 1; t < DEFAULT_MAP_LENGTH; t++){
          if( map_tiles[i][t][1] -> getType() == TILE_TEMP_WATER){
            if( wideningDir == 1){
              map_tiles[i - 1][t][1] -> setType( TILE_TEMP_WATER);
            }
            else{
              map_tiles[i][t - 1][1] -> setType( TILE_TEMP_WATER);
            }
          }
        }
      }
    }

    // Temp water to water
    for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
      for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
        if( map_tiles[i][t][1] -> getType() == TILE_TEMP_WATER){
          map_tiles[i][t][1] -> setType( TILE_WATER);
        }
      }
    }
  }

  // STEP 6:
  // Raise land
  quickPeek( "Creating height");

  int mountain_frequency = 0;
  int mountain_height = 0;
  int mountain_radius = 0;
  int mountain_steepness = 0;

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 2; u <  DEFAULT_MAP_HEIGHT; u++){
        // Grassland
        if( map_tiles[i][t][u] -> getBiome() == BIOME_GRASSLAND){
          mountain_frequency = 80;
          mountain_height = 2;
          mountain_radius = 4;
          mountain_steepness = 5;
        }
        // Desert
        else if( map_tiles[i][t][u] -> getBiome() == BIOME_DESERT){
          mountain_frequency = 90;
          mountain_height = 1;
          mountain_radius = 10;
          mountain_steepness = 5;
        }
        // Barren
        else if( map_tiles[i][t][u] -> getBiome() == BIOME_BARREN){
          mountain_frequency = 100;
          mountain_height = 8;
          mountain_radius = 4;
          mountain_steepness = 0;
        }
        // Tundra
        else if( map_tiles[i][t][u] -> getBiome() == BIOME_TUNDRA){
          mountain_frequency = 500;
          mountain_height = 1;
          mountain_radius = 5;
          mountain_steepness = 5;
        }
        // Forest
        else if( map_tiles[i][t][u] -> getBiome() == BIOME_FOREST){
          mountain_frequency = 40;
          mountain_height = 3;
          mountain_radius = 5;
          mountain_steepness = 2;
        }
        // Lake
        else if( map_tiles[i][t][u] -> getBiome() == BIOME_LAKE){
          mountain_frequency = 0;
          mountain_height = 0;
          mountain_radius = 0;
          mountain_steepness = 0;
        }
        // Other?
        else{
          mountain_frequency = 0;
          mountain_height = 0;
          mountain_radius = 0;
          mountain_steepness = 0;
        }

        // Make those mountains
        if(random(0, mountain_frequency) == 1 && map_tiles[i][t][u-1] -> getType() == TILE_GRASS){
          int mountainRaduis = random(0, mountain_radius);
          int mountainHeight = random(0, mountain_height);

          for(int w = 0; w < mountainHeight; w ++){
            if(u + w < DEFAULT_MAP_HEIGHT){
              for(int q = -mountainRaduis; q < mountainRaduis; q++){
                for(int r = -mountainRaduis; r < mountainRaduis; r++){
                  if(i + q < DEFAULT_MAP_WIDTH && i + q >= 0 && t + r < DEFAULT_MAP_LENGTH && t + r >= 0){
                    map_tiles[i + q][t + r][u + w] -> setType(TILE_GRASS);
                  }
                }
              }
              mountainRaduis -= mountain_steepness;
            }
          }
        }
      }
    }
  }

  // STEP6b:
  // Fill in holes
  quickPeek( "Filling holes");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = DEFAULT_MAP_HEIGHT - 1; u > 1; u--){
        if( map_tiles[i][t][u] -> getType() == TILE_GRASS && map_tiles[i][t][u-1] -> getType() == TILE_AIR){
          map_tiles[i][t][u-1] -> setType( TILE_GRASS);
        }
      }
    }
  }

  // STEP 7:
  // Create biome resources
  quickPeek( "Creating biome resources");

  // Frequencies per biome
  int tree_frequency = 0;
  int rock_frequency = 0;
  int cactus_frequency = 0;
  int tallgrass_frequency = 0;
  int nothing_frequency = 0;
  int snow_frequency = 0;

  int numberObjects = 6;

  // Place objects
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 1; u <  DEFAULT_MAP_HEIGHT - 1; u++){
        if( map_tiles[i][t][u-1] -> getType() == TILE_GRASS && map_tiles[i][t][u] -> getType() == TILE_AIR){
          // Reset Frequencies
          tree_frequency = 0;
          rock_frequency = 0;
          cactus_frequency = 0;
          tallgrass_frequency = 0;
          nothing_frequency = 0;
          snow_frequency = 0;

          // Grassland
          if( map_tiles[i][t][u] -> getBiome() == BIOME_GRASSLAND){
            tree_frequency = 60;
            rock_frequency = 80;
            tallgrass_frequency = 3;
            nothing_frequency = 90;
          }
          // Desert
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_DESERT){
            rock_frequency = 50;
            cactus_frequency = 60;
            nothing_frequency = 2;
          }
          // Barren
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_BARREN){
            rock_frequency = 10;
            tallgrass_frequency = 80;
            nothing_frequency = 2;
          }
          // Tundra
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_TUNDRA){
            rock_frequency = 80;
            snow_frequency = 2;
            nothing_frequency = 50;
          }
          // Forest
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_FOREST){
            tree_frequency = 2;
            rock_frequency = 80;
            tallgrass_frequency = 20;
            nothing_frequency = 30;
          }
          // Lake
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_LAKE){
            nothing_frequency = 1;
          }

          // Place some stuff
          bool objectPlaced = false;
          int randomGenerateSpawn = 0;
          while( !objectPlaced){
            randomGenerateSpawn = random(1, numberObjects);
            // Rock
            if(randomGenerateSpawn == 1 && random(1, rock_frequency) == 1){
              map_tiles[i][t][u] -> setType(TILE_ROCK);
              objectPlaced = true;
            }
            // Tree
            else if(randomGenerateSpawn == 2 && random(1, tree_frequency) == 1){
              map_tiles[i][t][u] -> setType(TILE_TREE);
              objectPlaced = true;
            }
            // Cactus
            else if(randomGenerateSpawn == 3 && random(1, cactus_frequency) == 1){
              map_tiles[i][t][u] -> setType(TILE_CACTUS);
              objectPlaced = true;
            }
            // Tallgrass
            else if(randomGenerateSpawn == 4 && random(1, tallgrass_frequency) == 1){
              map_tiles[i][t][u] -> setType(TILE_TALLGRASS);
              objectPlaced = true;
            }
            // Snow
            else if(randomGenerateSpawn == 5 && random(1, snow_frequency) == 1){
              map_tiles[i][t][u] -> setType(TILE_SNOW);
              objectPlaced = true;
            }
            // Clear space
            else if(randomGenerateSpawn == 6 && random(1, nothing_frequency) == 1){
              objectPlaced = true;
            }
          }
        }
      }
    }
  }

  // STEP 8:
  // Change water depending on biome
  quickPeek( "Changing water up");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        // Water, to ice or lava or none
        if( map_tiles[i][t][u] -> getType() == TILE_WATER){
          if( map_tiles[i][t][u] -> getBiome() == BIOME_TUNDRA)
            map_tiles[i][t][u] -> setType( TILE_ICE);
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_BARREN)
            map_tiles[i][t][u] -> setType( TILE_LAVA);
        }
      }
    }
  }

  // STEP 9:
  // Set biome tiles
  quickPeek( "Setting corresponding biome images");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        // Dirt, to sand or rock or snow
        if( map_tiles[i][t][u] -> getType() == TILE_GRASS){
          if( map_tiles[i][t][u] -> getBiome() == BIOME_TUNDRA)
            map_tiles[i][t][u] -> setType( TILE_GRASS_SNOW);
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_BARREN)
            map_tiles[i][t][u] -> setType( TILE_STONE);
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_LAKE)
            map_tiles[i][t][u] -> setType( TILE_WATER);
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_DESERT)
            map_tiles[i][t][u] -> setType( TILE_SAND);
        }
      }
    }
  }

  // STEP 10:
  // Removing "Water Mountains"
  quickPeek( "Carving mountains");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      if( map_tiles[i][t][1] -> getType() == TILE_WATER || map_tiles[i][t][1] -> getType() == TILE_LAVA || map_tiles[i][t][1] -> getType() == TILE_ICE){
        for(int u = 2; u < DEFAULT_MAP_HEIGHT; u++){
          map_tiles[i][t][u] -> setType( TILE_AIR);
        }
      }
    }
  }


  // Done!
  quickPeek( "Done!");

  // Sets all the tiles images
  refreshTileImages();
}

// Checks how many tiles dont have a biome
long tile_map::checkBiomeless(){
  long noBiome = 0;
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++)
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++)
      if( map_tiles[i][t][0] -> getBiome() == BIOME_NONE)
        noBiome++;
  return noBiome;
}

// Quick Peek
void tile_map::quickPeek( std::string currentPhase){
  refreshTileImages();
  rectfill( buffPoint, 0, 0, SCREEN_W, SCREEN_H, makecol( 0, 0, 0));
  draw( 0);
  textprintf_ex( buffPoint,font, 0, 0,makecol(0,0,0),makecol(255,255,255),"%s", currentPhase.c_str());
  draw_sprite( screen, buffPoint, 0, 0);
}

// Sets all the tiles images
void tile_map::refreshTileImages(){
  // Set images
  for(int i = 0; i < DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t < DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        // Set tile images
        if( tile_images[map_tiles[i][t][u] -> getType()][0] != errorTile){
          if( tile_images[map_tiles[i][t][u] -> getType()][1] != errorTile)
            map_tiles[i][t][u] -> setImages(tile_images[map_tiles[i][t][u] -> getType()][0], tile_images[map_tiles[i][t][u] -> getType()][1]);
          else
            map_tiles[i][t][u] -> setImages(tile_images[map_tiles[i][t][u] -> getType()][0], tile_images[map_tiles[i][t][u] -> getType()][0]);
        }
      }
    }
  }
}

//Draw map
void tile_map::draw( int newAnimationFrame){
  for(int i = 0; i < DEFAULT_MAP_WIDTH; i++){
    for(int t = DEFAULT_MAP_LENGTH - 1; t >= 0; t--){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        map_tiles[i][t][u] -> draw( buffPoint, newAnimationFrame, zoom, x, y);
      }
    }
  }
}
