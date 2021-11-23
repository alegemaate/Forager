/*
  Tile Type
  Allan Legemaate
  24/11/15
  Definitions of the tiles in game
*/

#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include <allegro.h>

#include <alleggl.h>
#include <string>

#include <GL/glu.h>

#include "../constants/globals.h"
#include "../constants/ids.h"
#include "../utils/quick-primatives.h"
#include "MaterialManager.h"

class TileType {
 public:
  TileType(const std::string& newName,
           unsigned char newType,
           const std::string& newModel,
           unsigned char newAttribute = 0,
           char newRandomness = 0);

  // Load image from file
  void setImages(const std::string& path1, const std::string& path2);

  // Get type
  unsigned char getType() const { return type; }

  // Get model
  unsigned char getRandomness() const { return randomness; }

  // Get type
  unsigned char getAttribute() const { return attribute; }

  // Draw
  void draw(float x, float y, float z, bool newTick);

 private:
  unsigned char type;
  unsigned char attribute;

  char randomness;

  GLuint image_reference_number[2];

  std::string model;
};

#endif  // TILE_TYPE_H
