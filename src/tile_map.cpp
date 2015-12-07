#include "tile_map.h"

// Construct
tile_map::tile_map( BITMAP *tempBuffer){
  // Starting position and zoom
  //x = 620;
  //y = 7880;
  x = 0;
  y = 0;
  z = 2;
  zoom = 16;

  test_x = test_y = test_z = 0;
  sel_x = sel_y = sel_z = 0;
  gameMode = false;

  // Buffer
  buffPoint = tempBuffer;

  // Load biomes
  biomes.load( "data/biomes.xml");

  // Load tiles
  tile_defs.load( "data/tiles.xml");

  // Make a mapful of tiles
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        map_tiles[i][t][u] = new tile( i, u, t, tile_defs.getTileByType(0));
      }
    }
  }
}

// Deconstruct
tile_map::~tile_map(){
  //dtor
}

// Check if tile is on screen
bool tile_map::onScreen( int tile_x, int tile_y, int tile_z){
  if(((tile_x + tile_z) + x) >= -120 * zoom &&
     ((tile_x  + tile_z) + x) <= SCREEN_W * zoom &&
     ((tile_x/2 - tile_z/2) + y - tile_y) >= -120 * zoom &&
     ((tile_x/2 - tile_z/2) + y - tile_y) <= SCREEN_H * zoom)
      return true;
  return false;
}

// Load images for tiles
void tile_map::load_images(){
  // Error
  /*errorTile = load_bitmap("images/tiles/NULL.png", NULL);

  // In case of error
  for( int i = 0; i < 100; i ++){
    for( int t = 0; t < 2; t ++){
      tile_images[i][t] = errorTile;
    }
  }

  // Static
  tile_images[TILE_AIR][0] = load_bitmap("images/tiles/air.png", NULL);
  tile_images[TILE_GRASS][0] = load_bitmap("images/tiles/grass.png", NULL);
  tile_images[TILE_ROCK][0] = load_bitmap("images/tiles/rock.png", NULL);
  tile_images[TILE_STONE][0] = load_bitmap("images/tiles/stone.png", NULL);
  tile_images[TILE_SAND][0] = load_bitmap("images/tiles/sand.png", NULL);
  tile_images[TILE_SNOW][0] = load_bitmap("images/tiles/snow.png", NULL);
  tile_images[TILE_ICE][0] = load_bitmap("images/tiles/ice.png", NULL);
  tile_images[TILE_CACTUS][0] = load_bitmap("images/tiles/cactus.png", NULL);
  tile_images[TILE_LAVA][0] = load_bitmap("images/tiles/lava.png", NULL);
  tile_images[TILE_TALLGRASS][0] = load_bitmap("images/tiles/tallgrass.png", NULL);
  tile_images[TILE_GRASS_SNOW][0] = load_bitmap("images/tiles/grass_snow.png", NULL);
  tile_images[TILE_TREE_PINE][0] = load_bitmap("images/tiles/tree_pine_1.png", NULL);
  tile_images[TILE_JOHNNY][0] = load_bitmap("images/tiles/johnny_small.bmp", NULL);

  // Animated
  tile_images[TILE_TREE][0] = load_bitmap("images/tiles/tree1.png", NULL);
  tile_images[TILE_TREE][1] = load_bitmap("images/tiles/tree2.png", NULL);

  tile_images[TILE_WATER][0] = load_bitmap("images/tiles/water1.png", NULL);
  tile_images[TILE_WATER][1] = load_bitmap("images/tiles/water2.png", NULL);*/

  // In case of error
  for( int i = 0; i < 20; i ++){
    overlay_images[i] = NULL;
  }

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
  // Right
  if(key[KEY_D] || key[KEY_RIGHT]){
    if( VIEW_MODE == 1){
      x += 0.5;
    }
    else{
      if( gameMode){
        if( test_y + 52 < DEFAULT_MAP_LENGTH){
          test_y += 1;
          x -= 32;
          y += 16;
        }
      }
      else
        x += 5 * zoom;
    }
  }
  // Left
  if(key[KEY_A] || key[KEY_LEFT]){
    if( VIEW_MODE == 1){
      x -= 0.5;
    }
    else{
      if( gameMode){
        if( test_y > 0){
          test_y -= 1;
          x += 32;
          y -= 16;
        }
      }
      else
        x += 5 * zoom;
    }
  }
  // Down
  if(key[KEY_S] || key[KEY_DOWN]){
    if( VIEW_MODE == 1){
      y -= 0.5;
    }
    else{
      if( gameMode){
        if( test_x + 52 < DEFAULT_MAP_WIDTH){
          test_x += 1;
          x -= 32;
          y -= 16;
        }
      }
      else
        y -= 5 * zoom;
    }
  }
  // Up
  if(key[KEY_W] || key[KEY_UP]){
    if( VIEW_MODE == 1){
      y += 0.5;
    }
    else{
      if( gameMode){
        if( test_x > 0){
          test_x -= 1;
          x += 32;
          y += 16;
        }
      }
      else
        y += 5 * zoom;
    }
  }

  // Zooming
  if( !gameMode){
    //Zoom out
    if(mouse_z < 0){
      if(zoom < 16){
        // Center zoom
        x += ((SCREEN_W) * zoom)/2 + (mouse_x - SCREEN_W/2) * zoom;
        y += ((SCREEN_H) * zoom)/2 + (mouse_y - SCREEN_H/2) * zoom;

        zoom *=2;
        rest(40);
      }
      else
        zoom = 16;
      position_mouse_z( 0);
    }
    //Zoom in
    if(mouse_z > 0){
      if(zoom > 1){
        zoom /=2;
        rest(40);

        // Center zoom
        x -= ((SCREEN_W) * zoom)/2 + (mouse_x - SCREEN_W/2) * zoom;
        y -= ((SCREEN_H) * zoom)/2 + (mouse_y - SCREEN_H/2) * zoom;
      }
      position_mouse_z( 0);
    }
  }

  // Change z slice DOWN
  if(( key[KEY_LSHIFT] && key[KEY_STOP] || key[KEY_MINUS_PAD]) && z > 0){
    z--;
    rest(100);
  }
  // Change z slice UP
  if(( key[KEY_LSHIFT] && key[KEY_COMMA] || key[KEY_PLUS_PAD]) && z < DEFAULT_MAP_HEIGHT - 1){
    z++;
    rest(100);
  }

  //Gen
  if(key[KEY_R]){
    generateMap();
  }

  // Game mode
  if( key[KEY_G]){
    if( !gameMode){
      x = -1000;
      y = 500;
      z = 2;
      zoom = 1;
    }
    gameMode = !gameMode;
    rest( 300);
  }

  // Erase tile
  if( mouse_b & 1)
    map_tiles[sel_x][sel_y][sel_z] -> setType( tile_defs.getTileByType(TILE_AIR));
}

// Procedural Generation of map
void tile_map::generateMap(){
  // STEP 1:
  // Start with air
  quickPeek( "Filling with Air");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u <  DEFAULT_MAP_HEIGHT; u++){
        map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_AIR));
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
        map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_GRASS));
      }
    }
  }

  // STEP 3:
  // Random Biomes
  quickPeek( "Placing Random Biome Spawns");

  //Biome spawn tiles
  for( int i = 0; i < ((DEFAULT_MAP_WIDTH * DEFAULT_MAP_LENGTH)/5000) + 1; i++){
    for( int k = 0; k < biomes.getNumberBiomes(); k++){
      if( random(0, biomes.getBiome(k).getChance()) == 1){
        map_tiles[random( 0, DEFAULT_MAP_WIDTH - 1)][random( 0, DEFAULT_MAP_LENGTH - 1)][0] -> setBiome( biomes.getBiome(k).getID());
      }
    }
  }

  // STEP 3b:
  // Place biome if there is none!
  if( checkBiomeNumber(BIOME_NONE) == DEFAULT_MAP_LENGTH * DEFAULT_MAP_WIDTH)
    map_tiles[0][0][0] -> setBiome( biomes.getBiome(1).getID());

  // Quick Peek
  quickPeek( "Filling Biomes");

  // Spread thoe biomes
  while( checkBiomeNumber( BIOME_NONE) > 0){
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
    textprintf_ex( screen,font, 0, 20,makecol(0,0,0),makecol(255,255,255),"Tiles Remaining: %10d", checkBiomeNumber( BIOME_NONE));
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
    while( !pathFound && checkBiomeNumber( BIOME_LAKE) > 0){
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
        map_tiles[river_x][river_y][1] -> setType( tile_defs.getTileByType(TILE_TEMP_WATER));
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
              map_tiles[i - 1][t][1] -> setType( tile_defs.getTileByType(TILE_TEMP_WATER));
            }
            else{
              map_tiles[i][t - 1][1] -> setType( tile_defs.getTileByType(TILE_TEMP_WATER));
            }
          }
        }
      }
    }

    // Temp water to water
    for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
      for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
        if( map_tiles[i][t][1] -> getType() == TILE_TEMP_WATER){
          map_tiles[i][t][1] -> setType( tile_defs.getTileByType(TILE_WATER));
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
  int mountain_steepness = 10;

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
          int mountainRaduis = random(1, mountain_radius);
          int mountainHeight = random(1, mountain_height);

          for(int w = 0; w < mountainHeight; w ++){
            if(u + w < DEFAULT_MAP_HEIGHT){
              for(int q = -mountainRaduis; q < mountainRaduis; q++){
                for(int r = -mountainRaduis; r < mountainRaduis; r++){
                  if(i + q < DEFAULT_MAP_WIDTH && i + q >= 0 && t + r < DEFAULT_MAP_LENGTH && t + r >= 0){
                    // Round mountains!
                    if( distanceTo2D(i, t, i + q, t + r) < (mountainRaduis - ceil(pow(double(mountain_steepness)/10, w)))){
                      map_tiles[i + q][t + r][u + w] -> setType( tile_defs.getTileByType(TILE_GRASS));
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

  // STEP6b:
  // Fill in holes
  quickPeek( "Filling holes");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = DEFAULT_MAP_HEIGHT - 1; u > 1; u--){
        if( map_tiles[i][t][u] -> getType() == TILE_GRASS && map_tiles[i][t][u-1] -> getType() == TILE_AIR){
          map_tiles[i][t][u-1] -> setType( tile_defs.getTileByType(TILE_GRASS));
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
          map_tiles[i][t][u] -> setType( tile_defs.getTileByType(biomes.getBiome(map_tiles[i][t][u] -> getBiome()).spawnResource()));
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
            map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_ICE));
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_BARREN)
            map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_LAVA));
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
            map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_GRASS_SNOW));
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_BARREN)
            map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_STONE));
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_LAKE)
            map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_WATER));
          else if( map_tiles[i][t][u] -> getBiome() == BIOME_DESERT)
            map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_SAND));
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
          map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_AIR));
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
long tile_map::checkBiomeNumber( char biomeToCheck){
  long numberOfBiome = 0;
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++)
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++)
      if( map_tiles[i][t][0] -> getBiome() == biomeToCheck)
        numberOfBiome++;
  return numberOfBiome;
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
  /*for(int i = 0; i < DEFAULT_MAP_WIDTH; i++){
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
  }*/
}

//Draw map
void tile_map::draw( int newAnimationFrame){
  // Number of tiles to iterate (changes for game mode)
  int iterate_x_start = 0;
  int iterate_x_end = DEFAULT_MAP_WIDTH;
  int iterate_y_start = DEFAULT_MAP_LENGTH - 1;
  int iterate_y_end = 1;

  if( gameMode){
    iterate_x_start = test_x;
    iterate_x_end = test_x + 52;
    iterate_y_start = test_y + 52 - 1;
    iterate_y_end = test_y;
  }

  for(int i = iterate_x_start; i < iterate_x_end; i++){
    for(int t = iterate_y_start; t >= iterate_y_end; t--){
      for(int n = 0; n < DEFAULT_MAP_HEIGHT; n++){
        // Draw tiles underneith with an overlay
        /*if(z > 1)
          map_tiles[i][t][z - 2] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_FOG_50]);
        if(z > 0)
          map_tiles[i][t][z - 1] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_NONE]);
*/

        // Draw tiles on current level (if onscreen)
        if( onScreen( map_tiles[i][t][n] -> getX(), map_tiles[i][t][n] -> getY(), map_tiles[i][t][n] -> getZ())){
          /*if( collisionAny(  mouse_x, mouse_x,
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
          else if( gameMode && map_tiles[i][t][n] -> getType() != TILE_AIR){
            if( distanceTo2D(sel_x, sel_y, i, t) > 20 )
              map_tiles[i][t][n] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_FOG_50]);
            else if( distanceTo2D(sel_x, sel_y, i, t) > 15 )
              map_tiles[i][t][n] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_FOG_25]);
            else if( distanceTo2D(sel_x, sel_y, i, t) > 10 )
              map_tiles[i][t][n] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_FOG_10]);
            else{
              map_tiles[i][t][n] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_NONE]);
            }
          }
          else{*/
            map_tiles[i][t][n] -> draw( buffPoint, newAnimationFrame, zoom, x, y, overlay_images[OVERLAY_NONE]);
          //}
        }
      }
    }
  }


  if( !gameMode){
    allegro_gl_set_allegro_mode();

    // Quick Info
    textprintf_ex( buffPoint,font,0, 80,makecol(0,0,0),makecol(255,255,255),"X:%i Y:%i Z:%i Zoom:%i", x, y, z, zoom);
    textprintf_ex( buffPoint,font,0, 120,makecol(0,0,0),makecol(255,255,255),"TX:%i TY:%i TZ:%i GameMode:%s", test_x, test_y, test_z, convertBoolToString(gameMode).c_str());

    // Tile info
    textprintf_ex( buffPoint,font,0, 160,makecol(0,0,0),makecol(255,255,255),"TILE INFO- Type:%s Biome:%s BiomeID:%i Temp:%i",
                  (tile_defs.getTileByType( map_tiles[sel_x][sel_y][sel_z] -> getType()) -> getName()).c_str(),
                  biomes.getBiome(map_tiles[sel_x][sel_y][sel_z] -> getBiome()).getName().c_str(),
                  map_tiles[sel_x][sel_y][sel_z] -> getBiome(),
                  biomes.getBiome(map_tiles[sel_x][sel_y][sel_z] -> getBiome()).getTemperature());

    allegro_gl_unset_allegro_mode();
    allegro_gl_flip();
  }
}
