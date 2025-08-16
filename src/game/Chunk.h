#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>

#include "../constants/globals.h"
#include "ChunkMesh.h"
#include "CubeFaces.h"
#include "Voxel.h"

class Chunk {
 public:
  Chunk(unsigned int x, unsigned int y, unsigned int z);

  // Generate chunk voxels
  void generate(int seed);

  // Get block
  Voxel& get(unsigned int x, unsigned int y, unsigned int z);

  // Set block
  void set(unsigned int x, unsigned int y, unsigned int z, unsigned char type);

  // Tessellate and such
  void update();

  // Render it all
  void render();

  // Position
  unsigned int getX() { return index_x; }
  unsigned int getY() { return index_y; }
  unsigned int getZ() { return index_z; }

 private:
  unsigned int index_x;
  unsigned int index_y;
  unsigned int index_z;

  Voxel blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]{};
  bool changed = false;

  // Data
  ChunkMesh mesh;
};

#endif  // CHUNK_H
