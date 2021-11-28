/*
  Tile Type
  Allan Legemaate
  24/11/15
  Definitions of the tiles in game
*/

#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include <GL/glew.h>
#include <string>

enum class ModelType {
  MODEL_NONE,
  MODEL_CUBE,
  MODEL_CUBE_SMALL,
  MODEL_FLAT,
  MODEL_DOUBLE_PLANE,
  MODEL_DOUBLE_PLANE_TALL,
};

class TileType {
 public:
  TileType(const std::string& newName,
           unsigned char newType,
           ModelType newModel,
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
  std::string name;
  unsigned char type;
  unsigned char attribute;
  ModelType model;
  char randomness;

  GLuint image_reference_number[2] = {};
};

#endif  // TILE_TYPE_H
