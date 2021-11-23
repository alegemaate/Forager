#include "Player.h"

#include "../utils/utils.h"

Player::Player(float x, float y, float z, float xRotation, float yRotation)
    : x(x), y(y), z(z), xRotation(xRotation), yRotation(yRotation) {}

// Apply world transformations to players
// Perspective
void Player::transformWorld() const {
  glRotatef(xRotation, 1.0, 0.0, 0.0);
  glRotatef(yRotation, 0.0, 1.0, 0.0);
  glTranslatef(-x, -y, -z);
}

// Draw tile
void Player::render() {
  // Nothing.. YET
}

// Move character and such
void Player::logic(TileMap* newMap) {
  // Move, slower in game mode
  // Sprint
  if (key[KEY_LSHIFT]) {
    sprinting = true;
  } else {
    sprinting = false;
  }

  // Pan around
  yRotation -= SCREEN_W / 2.0f - static_cast<float>(mouse_x);
  xRotation -= SCREEN_H / 2.0f - static_cast<float>(mouse_y);

  // No backflip
  if (xRotation > 90) {
    xRotation = 90;
  } else if (xRotation < -90) {
    xRotation = -90;
  }

  // Reset spinning amount
  if (yRotation >= 360) {
    yRotation -= 360;
  } else if (yRotation < 0) {
    yRotation += 360;
  }

  // Reset mouse pos
  position_mouse(SCREEN_W / 2, SCREEN_H / 2);

  // Collision
  bool canFall = true;

  // LEFT 1 RIGHT 2 FRONT 3 BACK 4
  bool canMoveXPLUS = true;
  bool canMoveXMINUS = true;
  bool canMoveZPLUS = true;
  bool canMoveZMINUS = true;

  if (gameMode) {
    for (int i = 0; i < DEFAULT_MAP_WIDTH; i++) {
      for (int t = 0; t < DEFAULT_MAP_LENGTH; t++) {
        for (int n = 0; n < DEFAULT_MAP_HEIGHT; n++) {
          if (newMap->getTile(i, t, n)->getTile()->getAttribute() !=
                  ATTRIBUTE_GAS &&
              newMap->getTile(i, t, n)->getTile()->getAttribute() !=
                  ATTRIBUTE_LIQUID) {
            // Check if near first
            if (distanceTo3D(x, y + y_velocity, z,
                             newMap->getTile(i, t, n)->getX(),
                             newMap->getTile(i, t, n)->getY(),
                             newMap->getTile(i, t, n)->getZ()) <= 2) {
              if (collision3d(x, 0.5f, newMap->getTile(i, t, n)->getX(), 0.5f,
                              y + y_velocity, 1.6f,
                              newMap->getTile(i, t, n)->getY(), 0.5f, z, 0.5f,
                              newMap->getTile(i, t, n)->getZ(), 0.5f)) {
                // Y Coords
                // Top
                if (collisionOverlap(y + y_velocity, 1.6f,
                                     newMap->getTile(i, t, n)->getY(), 0.5f)) {
                  canFall = false;
                  if (newMap->getTile(i, t, n)->getY() + 0.5f < y - 1.5f) {
                    y = newMap->getTile(i, t, n)->getY() + 0.5f + 1.5f;
                  }
                }
              }
              if (collision3d(x, 0.3, newMap->getTile(i, t, n)->getX(), 0.5,
                              y + y_velocity, 1.5,
                              newMap->getTile(i, t, n)->getY(), 0.5, z, 0.3,
                              newMap->getTile(i, t, n)->getZ(), 0.5)) {
                // X Coords
                // Left
                double buffer = 0.2f;
                if (collisionOverlap(x - buffer, 0.3,
                                     newMap->getTile(i, t, n)->getX(), 0.5)) {
                  canMoveXMINUS = false;
                  x += 0.005f;
                }
                // Right
                if (collisionOverlap(newMap->getTile(i, t, n)->getX(), 0.5,
                                     x + buffer, 0.3)) {
                  canMoveXPLUS = false;
                  x -= 0.005f;
                }

                // Z Coords
                // Front
                if (collisionOverlap(z - buffer, 0.3,
                                     newMap->getTile(i, t, n)->getZ(), 0.5)) {
                  canMoveZMINUS = false;
                  z += 0.005f;
                }
                // Back
                if (collisionOverlap(newMap->getTile(i, t, n)->getZ(), 0.5,
                                     z + buffer, 0.3)) {
                  canMoveZPLUS = false;
                  z -= 0.005f;
                }

                // Destroy Blocks FOR FUN
                if ((!canMoveZMINUS || !canMoveZPLUS || !canMoveXMINUS ||
                     !canMoveXPLUS)) {
                  if (mouse_b & 1) {
                    newMap->getTile(i, t, n)->setType(
                        TileTypeManager::getTileByType(TILE_GRASS));
                  }
                  if (mouse_b & 2) {
                    newMap->getTile(i, t, n)->setType(
                        TileTypeManager::getTileByType(TILE_AIR));
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  // Yep u can fall
  if (gameMode) {
    if (canFall) {
      y_velocity -= 0.01;
    }
    // Cant fall
    else if (key[KEY_SPACE] && y_velocity == 0) {
      y_velocity = 0.3;
    }
    // Stop falling if you can't fall
    else if (y_velocity < 0) {
      y_velocity = 0;
    }

    // Fall
    y += y_velocity;

    // Off edge
    if (y < -10) {
      x = DEFAULT_MAP_LENGTH / 2.0f;
      y = DEFAULT_MAP_HEIGHT;
      z = DEFAULT_MAP_WIDTH / 2.0f;
      xRotation = 45;
      yRotation = 135;
      y_velocity = 0;
    }
  }

  // Vars
  float yChange = -((float)sin(xRotation / 180 * M_PI) / (5 + 10 * gameMode) *
                    (sprinting + 1));
  float xChange = (float)sin(yRotation / 180 * M_PI) / (5 + 10 * gameMode) *
                  (sprinting + 1);
  float zChange = -(float)cos(yRotation / 180 * M_PI) / (5 + 10 * gameMode) *
                  (sprinting + 1);

  // Forward
  if (key[KEY_W] || key[KEY_UP]) {
    if (!gameMode) {
      y += yChange;
    }
    if (!gameMode || (zChange < 0 && canMoveZMINUS) ||
        (zChange > 0 && canMoveZPLUS)) {
      z += zChange;
    }
    if (!gameMode || (xChange < 0 && canMoveXMINUS) ||
        (xChange > 0 && canMoveXPLUS)) {
      x += xChange;
    }
  }
  // Backward
  if (key[KEY_S] || key[KEY_DOWN]) {
    if (!gameMode) {
      y -= yChange;
    }
    if (!gameMode || (zChange > 0 && canMoveZMINUS) ||
        (zChange < 0 && canMoveZPLUS)) {
      z -= zChange;
    }
    if (!gameMode || (xChange > 0 && canMoveXMINUS) ||
        (xChange < 0 && canMoveXPLUS)) {
      x -= xChange;
    }
  }
  // Left
  if (key[KEY_A] || key[KEY_LEFT]) {
    if (!gameMode || (xChange > 0 && canMoveZMINUS) ||
        (xChange < 0 && canMoveZPLUS)) {
      z -= xChange;
    }
    if (!gameMode || (zChange < 0 && canMoveXMINUS) ||
        (zChange > 0 && canMoveXPLUS)) {
      x += zChange;
    }
  }
  // Right
  if (key[KEY_D] || key[KEY_RIGHT]) {
    if (!gameMode || (xChange < 0 && canMoveZMINUS) ||
        (xChange > 0 && canMoveZPLUS)) {
      z += xChange;
    }
    if (!gameMode || (zChange > 0 && canMoveXMINUS) ||
        (zChange < 0 && canMoveXPLUS)) {
      x -= zChange;
    }
  }

  // Game mode
  if (key[KEY_G]) {
    if (gameMode) {
      x = 0;
      y = 15;
      z = 0;
      xRotation = 45;
      yRotation = 135;
    } else {
      x = DEFAULT_MAP_WIDTH / 2.0f;
      y = DEFAULT_MAP_HEIGHT;
      z = DEFAULT_MAP_LENGTH / 2.0f;
      xRotation = 45;
      yRotation = 135;
    }
    gameMode = !gameMode;
    rest(300);
  }
}
