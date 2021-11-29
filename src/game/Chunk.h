#ifndef CHUNK_H
#define CHUNK_H

#include "../constants/globals.h"
#include "ChunkMesh.h"
#include "CubeFaces.h"
#include "Voxel.h"

class Chunk {
 public:
  Chunk(unsigned int x, unsigned int z);

  // Generate chunk voxels
  void generate(int seed);

  // Get block
  Voxel* get(unsigned int x, unsigned int y, unsigned int z);

  // Set block
  void set(unsigned int x, unsigned int y, unsigned int z, unsigned char type);

  // Tessellate and such
  void update();

  // Render it all
  void render();

 private:
  unsigned int index_x;
  unsigned int index_z;

  Voxel* blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]{};
  bool changed = false;

  // Data
  ChunkMesh mesh;
};

#endif  // CHUNK_H
