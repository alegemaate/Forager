#include "tile_map.h"

// Construct
tile_map::tile_map( std::string newType, BITMAP *tempBuffer){
  // Starting position and zoom
  x = -1380;
  y = 4760;
  z = 2;
  zoom = 12;

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

  sel_x = sel_y = sel_z = 0;

  // Load biomes
  biomes.load( "data/biomes.xml");
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

  // In case of error
  for( int i = 0; i < 20; i ++){
    overlay_images[i] = errorTile;
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

  // Overlays
  overlay_images[OVERLAY_NONE] = NULL;
  overlay_images[OVERLAY_FOG_10] = load_bitmap("images/tiles/OVERLAY_FOG_10.png", NULL);
  overlay_images[OVERLAY_FOG_25] = load_bitmap("images/tiles/OVERLAY_FOG_25.png", NULL);
  overlay_images[OVERLAY_FOG_50] = load_bitmap("images/tiles/OVERLAY_FOG_50.png", NULL);
  overlay_images[OVERLAY_FOG_75] = load_bitmap("images/tiles/OVERLAY_FOG_75.png", NULL);
  overlay_images[OVERLAY_FOG_100] = load_bitmap("images/tiles/OVERLAY_FOG_100.png", NULL);
  overlay_images[OVERLAY_SELECTED] = load_bitmap("images/tiles/selected_overlay.png", NULL);

  // Sets all the tiles images
  refreshTileImages();
}

// Update map
void tile_map::update(){
  //Up
  if(key[KEY_UP]){
    y += 5 * zoom;
  }
  //Down
  if(key[KEY_DOWN]){
    y -= 5 * zoom;
  }
  //Left
  if(key[KEY_LEFT]){
    x += 5 * zoom;
  }
  //Right
  if(key[KEY_RIGHT]){
    x -= 5 * zoom;
  }
  //Zoom out
  if(key[KEY_Q] || mouse_z < 0){
    if(zoom < 16){
      zoom *=2;
      rest(40);

      // Center zoom
      x += ((SCREEN_W) * zoom)/4;
      y += ((SCREEN_H) * zoom)/4;
    }
    else
      zoom = 16;
    position_mouse_z( 0);
  }
  //Zoom in
  if(key[KEY_A] || mouse_z > 0){
    if(zoom > 1){
      zoom /=2;
      rest(40);

      // Center zoom
      x -= ((SCREEN_W) * zoom)/2;
      y -= ((SCREEN_H) * zoom)/2;
    }
    position_mouse_z( 0);
  }

  // Change z slice DOWN
  if(( key[KEY_LSHIFT] && key[KEY_COMMA] || key[KEY_MINUS_PAD]) && z > 0){
    z--;
    rest(100);
  }
  // Change z slice UP
  if(( key[KEY_LSHIFT] && key[KEY_STOP] || key[KEY_PLUS_PAD]) && z < DEFAULT_MAP_HEIGHT - 1){
    z++;
    rest(100);
  }

  //Gen
  if(key[KEY_R]){
    generateMap("normal");
  }

  // Erase tile
  if( mouse_b & 1)
    map_tiles[sel_x][sel_y][sel_z] -> setType( TILE_AIR, "Air");
}

// Procedural Generation of map
void tile_map::generateMap(std::string newType){
  // STEP 1:
  // Start with air
  quickPeek( "Filling with Air");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        map_tiles[i][t][u] -> setType(TILE_AIR, "Air");
        map_tiles[i][t][u] -> setBiome(BIOME_NONE, "None");
      }
    }
  }

  // STEP 2:
  // Fill with dirt
  quickPeek( "Placing dirt");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  2; u++){
        map_tiles[i][t][u] -> setType(TILE_GRASS, "Grass");
      }
    }
  }

  // STEP 3:
  // Random Biomes
  quickPeek( "Placing Random Biome Spawns");

  //Biome spawn tiles
  for( int i = 0; i < ((DEFAULT_MAP_WIDTH * DEFAULT_MAP_LENGTH)/6000) + 1; i++){
    for( int k = 0; k < biomes.getNumberBiomes(); k++){
      map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( biomes.getBiome(k).getID(), biomes.getBiome(k).getName());
    }
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
                map_tiles[i - 1][t][0] -> setBiome( map_tiles[i][t][0] -> getBiome(), map_tiles[i][t][0] -> getBiomeName());
              break;
            case 1:
              if( t > 0 && map_tiles[i][t - 1][0] -> getBiome() == BIOME_NONE)
                map_tiles[i][t - 1][0] -> setBiome( map_tiles[i][t][0] -> getBiome(), map_tiles[i][t][0] -> getBiomeName());
              break;
            case 2:
              if( i < DEFAULT_MAP_WIDTH - 1 && map_tiles[i + 1][t][0] -> getBiome() == BIOME_NONE)
                map_tiles[i + 1][t][0] -> setBiome( map_tiles[i][t][0] -> getBiome(), map_tiles[i][t][0] -> getBiomeName());
              break;
            case 3:
              if( t < DEFAULT_MAP_LENGTH - 1 && map_tiles[i][t + 1][0] -> getBiome() == BIOME_NONE)
                map_tiles[i][t + 1][0] -> setBiome( map_tiles[i][t][0] -> getBiome(), map_tiles[i][t][0] -> getBiomeName());
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
        map_tiles[i][t][u] -> setBiome( map_tiles[i][t][0] -> getBiome(), map_tiles[i][t][0] -> getBiomeName());
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
        map_tiles[river_x][river_y][1] -> setType(TILE_TEMP_WATER, "TEMP_WATER");
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
              map_tiles[i - 1][t][1] -> setType( TILE_TEMP_WATER, "TEMP_WATER");
            }
            else{
              map_tiles[i][t - 1][1] -> setType( TILE_TEMP_WATER, "TEMP_WATER");
            }
          }
        }
      }
    }

    // Temp water to water
    for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
      for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
        if( map_tiles[i][t][1] -> getType() == TILE_TEMP_WATER){
          map_tiles[i][t][1] -> setType( TILE_WATER, "Water");
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
        // Spawning mountains
        mountain_frequency = biomes.getBiome(map_tiles[i][t][u-1] -> getBiome()).getMountainFrequency();
        mountain_height = biomes.getBiome(map_tiles[i][t][u-1] -> getBiome()).getMountainHeight();
        mountain_radius = biomes.getBiome(map_tiles[i][t][u-1] -> getBiome()).getMountainRadius();
        mountain_steepness = biomes.getBiome(map_tiles[i][t][u-1] -> getBiome()).getMountainSteepness();

        // Make those mountains
        if(random(0, mountain_frequency) == 1 && map_tiles[i][t][u-1] -> getType() == TILE_GRASS){
          int mountainRaduis = random(0, mountain_radius);
          int mountainHeight = random(0, mountain_height);

          for(int w = 0; w < mountainHeight; w ++){
            if(u + w < DEFAULT_MAP_HEIGHT){
              for(int q = -mountainRaduis; q < mountainRaduis; q++){
                for(int r = -mountainRaduis; r < mountainRaduis; r++){
                  if(i + q < DEFAULT_MAP_WIDTH && i + q >= 0 && t + r < DEFAULT_MAP_LENGTH && t + r >= 0){
                    map_tiles[i + q][t + r][u + w] -> setType(TILE_GRASS, "Grass");
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
          map_tiles[i][t][u-1] -> setType( TILE_GRASS, "Grass");
        }
      }
    }
  }

  // STEP 7:
  // Create biome resources
  quickPeek( "Creating biome resources");

  // Place objects
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 1; u <  DEFAULT_MAP_HEIGHT - 1; u++){
        if( map_tiles[i][t][u-1] -> getType() == TILE_GRASS && map_tiles[i][t][u] -> getType() == TILE_AIR){
          // Spawn tile
          map_tiles[i][t][u] -> setType(biomes.getBiome(map_tiles[i][t][u] -> getBiome()).spawnResource(), "SPAWNEDRESOURCE");
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
            map_tiles[i][t][u] -> setType( TILE_ICE, "Ice");
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_BARREN)
            map_tiles[i][t][u] -> setType( TILE_LAVA, "Lava");
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
            map_tiles[i][t][u] -> setType( TILE_GRASS_SNOW, "Snowy Grass");
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_BARREN)
            map_tiles[i][t][u] -> setType( TILE_STONE, "Stone");
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_LAKE)
            map_tiles[i][t][u] -> setType( TILE_WATER, "Water");
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_DESERT)
            map_tiles[i][t][u] -> setType( TILE_SAND, "Sand");
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
          map_tiles[i][t][u] -> setType( TILE_AIR, "Air");
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
      for(int n = 0; n < DEFAULT_MAP_HEIGHT; n++){
      // Draw tiles underneith with an overlay
      /*if(z > 1)
        map_tiles[i][t][z - 2] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_FOG_50]);
      if(z > 0)
        map_tiles[i][t][z - 1] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_NONE]);
*/
      // Draw tiles on current level
      if( collisionAny(  mouse_x, mouse_x,
                      (( map_tiles[i][t][z] -> getX() + map_tiles[i][t][z] -> getZ()) * 64)/zoom + x/zoom,
                      (( map_tiles[i][t][z] -> getX() + map_tiles[i][t][z] -> getZ()) * 64)/zoom + x/zoom + 64/zoom,
                         mouse_y, mouse_y,
                      (( map_tiles[i][t][z] -> getX() - map_tiles[i][t][z] -> getZ()) * 64)/zoom/2 - (map_tiles[i][t][z] -> getY() * 64/zoom) + y/zoom,
                      (( map_tiles[i][t][z] -> getX() - map_tiles[i][t][z] -> getZ()) * 64)/zoom/2 - (map_tiles[i][t][z] -> getY() * 64/zoom) + y/zoom + 64/zoom)){
        map_tiles[i][t][z] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_SELECTED]);
        sel_x = i;
        sel_y = t;
        sel_z = z;
      }
      else{
        map_tiles[i][t][n] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_NONE]);
      }
    }
  }

  // Quick Info
  textprintf_ex( buffPoint,font,0, 100,makecol(0,0,0),makecol(255,255,255),"X:%i Y:%i Z:%i Zoom:%i", x, y, z, zoom);

  // Tile info
  textprintf_ex( buffPoint,font,0, 140,makecol(0,0,0),makecol(255,255,255),"TILE INFO- Type:%s Biome:%s BiomeID:%i Temp:%i",
                map_tiles[sel_x][sel_y][sel_z] -> getName().c_str(),
                map_tiles[sel_x][sel_y][sel_z] -> getBiomeName().c_str(),
                map_tiles[sel_x][sel_y][sel_z] -> getBiome(),
                map_tiles[sel_x][sel_y][sel_z] -> getTemperature());
  }
}
