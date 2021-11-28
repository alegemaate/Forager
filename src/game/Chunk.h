#ifndef CHUNK_H
#define CHUNK_H

#include "../constants/globals.h"
#include "CubeFaces.h"
#include "Tile.h"

class Chunk {
 public:
  Chunk(int x, int z);

  // Fill a given face
  void fillFace(FaceDefenition face[6], glm::vec3 offset, GLuint atlasPos);

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
  GLuint chunkVAO = 0;
  GLuint chunkVBO = 0;
  GLuint chunkEBO = 0;

  GLint maxIndices = 0;
  GLuint numIndices = 0;

  GLfloat* vertices;
  GLuint* indices;

  static GLuint atlas;
};

#endif  // CHUNK_H
