#include "player.h"

player::player( float newX, float newY, float newZ, float newXRot, float newYRot){
  x = newX;
  y = newY;
  z = newZ;
  xRotation = newXRot;
  yRotation = newYRot;

  y_velocity = 0;
}

player::~player(){

}

// Draw tile
void player::render(){
  // Nothing.. YET EHHEAHAEH
}

// Move character and such
void player::logic( tile_map *newMap){
// Move, slower in gamemode
  // Forward
  if(key[KEY_W] || key[KEY_UP]){
    if( !gameMode)
      y -= (float)sin(xRotation / 180 * M_PI)/(5 + 10 * gameMode);
    z -= (float)cos(yRotation  / 180 * M_PI)/(5 + 10 * gameMode);
    x += (float)sin(yRotation / 180 * M_PI)/(5 + 10 * gameMode);
  }
  // Backward
  if(key[KEY_S] || key[KEY_DOWN]){
    if( !gameMode)
      y += (float)sin(xRotation / 180 * M_PI)/(5 + 10 * gameMode);
    z += (float)cos(yRotation / 180 * M_PI)/(5 + 10 * gameMode);
    x -= (float)sin(yRotation / 180 * M_PI)/(5 + 10 * gameMode);
  }
  // Left
  if(key[KEY_A] || key[KEY_LEFT]){
    x -= (float)cos(yRotation / 180 * M_PI)/(5 + 10 * gameMode);
    z -= (float)sin(yRotation / 180 * M_PI)/(5 + 10 * gameMode);
  }
  // Right
  if(key[KEY_D] || key[KEY_RIGHT]){
    x += (float)cos(yRotation / 180 * M_PI)/(5 + 10 * gameMode);
    z += (float)sin(yRotation / 180 * M_PI)/(5 + 10 * gameMode);
  }

  // Pan around
  yRotation -= SCREEN_W/2 - mouse_x;
  xRotation -= SCREEN_H/2 - mouse_y;

  // No backflips!
  if( xRotation > 90)
    xRotation = 90;
  else if( xRotation < -90)
    xRotation = -90;

  // Reset spinning amount
  if( yRotation >= 360)
    yRotation -= 360;
  else if( yRotation < 0)
    yRotation += 360;

  // Reset mouse pos
  position_mouse(SCREEN_W/2, SCREEN_H/2);

  // Collision
  bool canFall = true;

  //std::cout << "X:" << x << " Y:" << y << " Z:" << z << " RotX:" << xRotation << " RotY:" << yRotation << "\n";
  if( gameMode){
    for(int i = 0; i < DEFAULT_MAP_WIDTH; i++){
      for(int t = 0; t < DEFAULT_MAP_LENGTH; t++){
        for(int n = 0; n < DEFAULT_MAP_HEIGHT; n++){
          if( canFall && (newMap -> map_tiles[i][t][n] -> getType() != TILE_AIR && newMap -> map_tiles[i][t][n] -> getType() != TILE_SNOW && newMap -> map_tiles[i][t][n] -> getType() != TILE_GRASS && newMap -> map_tiles[i][t][n] -> getType() != TILE_TREE)){
            // Check if near first
            if( distanceTo3D( x, y, z, newMap -> map_tiles[i][t][n] -> getX(), newMap -> map_tiles[i][t][n] -> getY(), newMap -> map_tiles[i][t][n] -> getZ()) <= 2){
              if( collision3d( x, 1, newMap -> map_tiles[i][t][n] -> getX(), 1, y, 1, newMap -> map_tiles[i][t][n] -> getY(), 1, z, 1, newMap -> map_tiles[i][t][n] -> getZ(), 1)){
                canFall = false;
              }
            }
          }
        }
      }
    }
  }

  // Yep u can fall
  if( gameMode){
    if( canFall){
      y -= 0.2;
      if( y < -10){
        x = DEFAULT_MAP_LENGTH/2;
        y = DEFAULT_MAP_HEIGHT;
        z = DEFAULT_MAP_WIDTH/2;
        xRotation = 45;
        yRotation = 135;
      }
    }
    // Cant fall
    else if( key[KEY_SPACE] && y_velocity == 0){
      y_velocity = 0.5;
    }
    y += y_velocity;
    if( y_velocity > 0)
      y_velocity *= 0.90;
    if( y_velocity < 0.01)
      y_velocity = 0;
  }


  // Game mode
  if( key[KEY_G]){
    if( gameMode){
      x = 0;
      y = 15;
      z = 0;
      xRotation = 45;
      yRotation = 135;
    }
    else{
      x = DEFAULT_MAP_LENGTH/2;
      y = DEFAULT_MAP_HEIGHT;
      z = DEFAULT_MAP_WIDTH/2;
      xRotation = 45;
      yRotation = 135;
    }
    gameMode = !gameMode;
    rest( 300);
  }
}
