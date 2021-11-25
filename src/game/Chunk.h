#ifndef CHUNK_H
#define CHUNK_H

#include <alleggl.h>

#include "../constants/globals.h"
#include "Tile.h"

class Chunk {
 public:
  Chunk(int x, int z);

  // Fill array with given data
  static void fillArray(glm::vec3 posVec,
                        glm::vec3 normVec,
                        glm::vec2 texVec,
                        GLfloat* newArray,
                        unsigned long index);

  // Tessellate chunk
  void tessellate();

  void generate(int seed);

  // Get block
  Tile* get(unsigned int x, unsigned int y, unsigned int z);

  // Set block
  void set(unsigned int x, unsigned int y, unsigned int z, unsigned char type);

  // Tessellate and such
  void update();

  // Render it all
  void render();

 private:
  int index_x, index_z;

  Tile* blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]{};
  bool changed = false;

  // Data
  GLuint geometry_array = 0;
  GLuint indices_array = 0;

  int num_indices;

  GLfloat* geometry;
  unsigned long* indices;
};

#endif  // CHUNK_H
