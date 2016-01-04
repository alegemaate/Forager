#include "tile_map.h"

// Construct
tile_map::tile_map( BITMAP *tempBuffer){
  // Starting position and zoom
  x = 0;
  y = -15;
  z = 0;
  rot_x = 45;
  rot_y = 135;
  rot_z = 0;
  zoom = 0;

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
}

// Update map
void tile_map::update(){
  // Move, slower in gamemode
  // Forward
  if(key[KEY_W] || key[KEY_UP]){
    z += (float)cos(rot_y * M_PI / 180)/(5 + 10 * gameMode);
    x -= (float)sin(rot_y * M_PI / 180)/(5 + 10 * gameMode);
    y += (float)sin(rot_x * M_PI / 180)/(5 + 10 * gameMode);
  }
  // Backward
  if(key[KEY_S] || key[KEY_DOWN]){
    z -= (float)cos(rot_y * M_PI / 180)/(5 + 10 * gameMode);
    x += (float)sin(rot_y * M_PI / 180)/(5 + 10 * gameMode);
    y -= (float)sin(rot_x * M_PI / 180)/(5 + 10 * gameMode);
  }
  // Left
  if(key[KEY_A] || key[KEY_LEFT]){
    x += (float)cos(rot_y * M_PI / 180)/(5 + 10 * gameMode);
    z += (float)sin(rot_y * M_PI / 180)/(5 + 10 * gameMode);
  }
  // Right
  if(key[KEY_D] || key[KEY_RIGHT]){
    x -= (float)cos(rot_y * M_PI / 180)/(5 + 10 * gameMode);
    z -= (float)sin(rot_y * M_PI /180)/(5 + 10 * gameMode);
  }

  // Pan around
  rot_y -= SCREEN_W/2 - mouse_x;
  rot_x -= SCREEN_H/2 - mouse_y;

  // No backflips!
  if( rot_x > 90)
    rot_x = 90;
  else if( rot_x < -90)
    rot_x = -90;

  // Reset spinning amount
  if( rot_y >= 360)
    rot_y -= 360;
  else if( rot_y < 0)
    rot_y += 360;

  // Reset mouse pos
  position_mouse(SCREEN_W/2, SCREEN_H/2);

  // Zooming
  //Zoom out
  if(mouse_z < 0){
    zoom -= 0.5;
    if( !gameMode )
      zoom -= 0.5;
    position_mouse_z( 0);
  }
  //Zoom in
  if(mouse_z > 0){
    zoom += 0.5;
    if( !gameMode )
      zoom += 0.5;
    position_mouse_z( 0);
  }

  //Gen
  if(key[KEY_R]){
    generateMap();
  }

  // Game mode
  if( key[KEY_G]){
    if( !gameMode){
      x = -15;
      y = -3;
      z = -15;
      rot_x = 15;
      rot_y = 135;
      rot_z = 0;
    }
    else{
      x = 0;
      y = -15;
      z = 0;
      rot_x = 45;
      rot_y = 135;
      rot_z = 0;
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
  // GENERATE MAP
  std::cout << "GENERATING MAP\n-------------\n";

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
    //std::cout << "Tiles Remaining:" << checkBiomeNumber( BIOME_NONE) << "\n";
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
    std::cout << "Rivers Remaining:" << numberRivers-r << "\n";

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
        map_tiles[river_x][river_y][1] -> setType( tile_defs.getTileByType(TILE_TEMP));
      }

      // Shift river
      // Zero in on final destiantionposition_mouse_z( 0);
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
          if( map_tiles[i][t][1] -> getType() == TILE_TEMP){
            if( wideningDir == 1){
              map_tiles[i - 1][t][1] -> setType( tile_defs.getTileByType(TILE_TEMP));
            }
            else{
              map_tiles[i][t - 1][1] -> setType( tile_defs.getTileByType(TILE_TEMP));
            }
          }
        }
      }
    }

    // Temp water to water
    for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
      for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
        if( map_tiles[i][t][1] -> getType() == TILE_TEMP){
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

  // STEP 6c:
  // Slating "Stuff from Under"
  quickPeek( "Slating Stuff from Under");

  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u < DEFAULT_MAP_HEIGHT; u++){
        bool slatedForRemoval = true;
        if( i > 0 && map_tiles[i - 1][t][u] -> getType() == TILE_AIR)
          slatedForRemoval = false;
        else if( t > 0 && map_tiles[i][t - 1][u] -> getType() == TILE_AIR)
          slatedForRemoval = false;
        else if( u > 0 && map_tiles[i][t][u - 1] -> getType() == TILE_AIR)
          slatedForRemoval = false;
        else if( i < DEFAULT_MAP_WIDTH - 1 && map_tiles[i + 1][t][u] -> getType() == TILE_AIR)
          slatedForRemoval = false;
        else if( t < DEFAULT_MAP_LENGTH - 1 && map_tiles[i][t + 1][u] -> getType() == TILE_AIR)
          slatedForRemoval = false;
        else if( u < DEFAULT_MAP_HEIGHT - 1 && map_tiles[i][t][u + 1] -> getType() == TILE_AIR)
          slatedForRemoval = false;

        if( slatedForRemoval)
          map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_TEMP));
      }
    }
  }

  // STEP 6d:
  // Removing "Stuff from Under"
  quickPeek( "Removing Stuff from Under");
  for(int i = 0; i <  DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t <  DEFAULT_MAP_LENGTH; t++){
      for(int u = 0; u < DEFAULT_MAP_HEIGHT; u++){
        if( map_tiles[i][t][u] -> getType() == TILE_TEMP)
          map_tiles[i][t][u] -> setType( tile_defs.getTileByType(TILE_AIR));
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
  /*allegro_gl_set_allegro_mode();
  rectfill( buffPoint, 0, 0, SCREEN_W, SCREEN_H, makecol( 0, 0, 0));
  textprintf_ex( buffPoint,font, 0, 0,makecol(0,0,0),makecol(255,255,255),"%s", currentPhase.c_str());
  draw_sprite( screen, buffPoint, 0, 0);
  allegro_gl_unset_allegro_mode();*/
  std::cout << "PHASE:" << currentPhase.c_str() << "\n";
  draw( 0);
}

//Draw map
void tile_map::draw( int newAnimationFrame){
  // TRANSFORMS
  // Reset camera transforms
  glLoadIdentity();

  // Rotate along x
  glRotatef( rot_x, 1.0, 0.0, 0.0 );

  // Rotate along y
  glRotatef( rot_y, 0.0, 1.0, 0.0 );

  // Rotate along z (unused)
  //glRotatef( rot_z, 0.0, 0.0, 1.0 );

  // Translate map
  glTranslatef( x, y, z);

  // Place light 0 (Direction)
  GLfloat light_position[] = { 1.0001, 1.0001, 1.0001, 0.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // Place light 1 (Direction2)
  GLfloat light_position2[] = { -1.0001, -1.0001, -1.0001, 0.0f };
  glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

  // Go through all tiles and draw
  for(int i = 0; i < DEFAULT_MAP_WIDTH; i++){
    for(int t = 0; t < DEFAULT_MAP_LENGTH; t++){
      for(int n = 0; n < DEFAULT_MAP_HEIGHT; n++){
        // Draw tiles on current level (if onscreen)
        if( VIEW_MODE == 1 || onScreen( map_tiles[i][t][n] -> getX(), map_tiles[i][t][n] -> getY(), map_tiles[i][t][n] -> getZ())){
          map_tiles[i][t][n] -> draw( buffPoint, newAnimationFrame, overlay_images[OVERLAY_NONE]);
        }
      }
    }
  }



  /*if( !gameMode){
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
  }*/
}
