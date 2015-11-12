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
    x += 64;
    y += 32;
    rest(40);
  }
  //Down
  if(key[KEY_DOWN]){
    x -= 64;
    y -= 32;
    rest(40);
  }
  //Left
  if(key[KEY_LEFT]){
    x += 64;
    y -= 32;
    rest(40);
  }
  //Right
  if(key[KEY_RIGHT]){
    x -= 64;
    y += 32;
    rest(40);
  }
  //Zoom in
  if(key[KEY_Q]){
    zoom ++;
    while(key[KEY_Q]){
    }
  }
  //Zoom out
  if(key[KEY_A]){
    if(zoom > 1){
      zoom --;
    }
    while(key[KEY_A]){
    }
  }
  //Gen
  if(key[KEY_R]){
    generateMap("normal");
    //while(key[KEY_R]){
    //}
  }
}

// Procedural Generation of map
void tile_map::generateMap(std::string newType){
  // STEP 1:
  // Start with air
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
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT - 1; u++){
        map_tiles[i][t][u] -> setType(TILE_GRASS);
      }
    }
  }

  // Quick Peek
  quickPeek();

  // STEP 3:
  // Make River
  int numberRivers = 1; //random( 0, 2);

  for( int r = 0; r < numberRivers; r++){
    int river_x = random( 0, int(DEFAULT_MAP_WIDTH * DEFAULT_MAP_HEIGHT)/1600);
    int river_y = 0;

    // Carve river
    while( river_y < DEFAULT_MAP_LENGTH){
      if( river_x < DEFAULT_MAP_WIDTH && river_y < DEFAULT_MAP_LENGTH && river_x >= 0 && river_y >= 0){
        map_tiles[river_x][river_y][1] -> setType(TILE_WATER);
      }

      // Shift river
      if( random( 1, 10) == 1){
        river_x += 1;
      }
      if( random( 1, 10) == 1){
        river_x -= 1;
      }
      river_y ++;
    }

    // Widen river
    int river_width = random( 1, 10);

    for( int q = 0; q < river_width; q++){
      int wideningDirection = random( 0, 3);
      for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
        for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
          if( map_tiles[i][t][1] -> getType() == TILE_WATER){
            switch( wideningDirection){
              case 0:
                if( i > 0)
                  map_tiles[i - 1][t][1] -> setType( TILE_TEMP_WATER);
                break;
              case 1:
                if( t > 0)
                  map_tiles[i][t - 1][1] -> setType( TILE_TEMP_WATER);
                break;
              case 2:
                if( i < DEFAULT_MAP_WIDTH - 1)
                  map_tiles[i + 1][t][1] -> setType( TILE_TEMP_WATER);
                break;
              case 3:
                if( t < DEFAULT_MAP_LENGTH - 1)
                  map_tiles[i][t + 1][1] -> setType( TILE_TEMP_WATER);
                break;
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

  // Quick Peek
  quickPeek();

  // STEP 4:
  // Random Biomes

  //Biome spawn tiles
  for( int i = 0; i < (DEFAULT_MAP_WIDTH * DEFAULT_MAP_LENGTH)/5000; i++){
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_BARREN);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_TUNDRA);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_GRASSLAND);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_TUNDRA);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_DESERT);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_FOREST);
    map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( BIOME_LAKE);
  }

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
    textprintf_ex( screen,font, 0, 0,makecol(0,0,0),makecol(255,255,255),"%i", checkBiomeless());
  }

  // Quick Peek
  quickPeek();

  // Extend through all tiles
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        map_tiles[i][t][u] -> setBiome( map_tiles[i][t][0] -> getBiome());
      }
    }
  }

  // Quick Peek
  quickPeek();

  // STEP 5:
  // Set biome tiles
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

        // Dirt, to sand or rock or snow
        if( map_tiles[i][t][u] -> getType() == TILE_GRASS){
          if( map_tiles[i][t][u] -> getBiome() == BIOME_TUNDRA)
            map_tiles[i][t][u] -> setType( TILE_SNOW);
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

  // Quick Peek
  quickPeek();

  // STEP 6:
  // Create biome resources
  // Frequencies per biome
  int tree_frequency = 0;
  int rock_frequency = 0;
  int cactus_frequency = 0;
  int tallgrass_frequency = 0;
  int nothing_frequency = 0;

  int numberObjects = 5;

  // Place objects
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      if( map_tiles[i][t][1] -> getType() != TILE_WATER && map_tiles[i][t][1] -> getType() != TILE_AIR && map_tiles[i][t][1] -> getType() != TILE_LAVA){
        // Grassland
        if( map_tiles[i][t][2] -> getBiome() == BIOME_GRASSLAND){
          tree_frequency = 60;
          rock_frequency = 80;
          cactus_frequency = 0;
          tallgrass_frequency = 3;
          nothing_frequency = 90;
        }
        // Desert
        else if( map_tiles[i][t][2] -> getBiome() == BIOME_DESERT){
          tree_frequency = 0;
          rock_frequency = 50;
          cactus_frequency = 60;
          tallgrass_frequency = 0;
          nothing_frequency = 2;
        }
        // Barren
        else if( map_tiles[i][t][2] -> getBiome() == BIOME_BARREN){
          tree_frequency = 0;
          rock_frequency = 10;
          cactus_frequency = 0;
          tallgrass_frequency = 80;
          nothing_frequency = 2;
        }
        // Tundra
        else if( map_tiles[i][t][2] -> getBiome() == BIOME_TUNDRA){
          tree_frequency = 0;
          rock_frequency = 80;
          cactus_frequency = 0;
          tallgrass_frequency = 0;
          nothing_frequency = 2;
        }
        // Forest
        else if( map_tiles[i][t][2] -> getBiome() == BIOME_FOREST){
          tree_frequency = 2;
          rock_frequency = 80;
          cactus_frequency = 0;
          tallgrass_frequency = 20;
          nothing_frequency = 30;
        }
        // Lake
        else if( map_tiles[i][t][2] -> getBiome() == BIOME_LAKE){
          tree_frequency = 0;
          rock_frequency = 0;
          cactus_frequency = 0;
          tallgrass_frequency = 0;
          nothing_frequency = 1;
        }

        // Place some stuff
        bool objectPlaced = false;
        int randomGenerateSpawn = 0;
        while( !objectPlaced){
          randomGenerateSpawn = random(1, numberObjects);
          // Rock
          if(randomGenerateSpawn == 1 && random(1, rock_frequency) == 1){
            map_tiles[i][t][2] -> setType(TILE_ROCK);
            objectPlaced = true;
          }
          // Tree
          else if(randomGenerateSpawn == 2 && random(1, tree_frequency) == 1){
            map_tiles[i][t][2] -> setType(TILE_TREE);
            objectPlaced = true;
          }
          // Cactus
          else if(randomGenerateSpawn == 3 && random(1, cactus_frequency) == 1){
            map_tiles[i][t][2] -> setType(TILE_CACTUS);
            objectPlaced = true;
          }
          // Tallgrass
          else if(randomGenerateSpawn == 4 && random(1, tallgrass_frequency) == 1){
            map_tiles[i][t][2] -> setType(TILE_TALLGRASS);
            objectPlaced = true;
          }
          // Clear space
          else if(randomGenerateSpawn == 5 && random(1, nothing_frequency) == 1){
            objectPlaced = true;
          }
        }
      }
    }
  }

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
void tile_map::quickPeek(){
  refreshTileImages();
  draw( 0);
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
