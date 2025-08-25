#pragma once

#include <glm/glm.hpp>

#include "../core/SimplexNoise.h"
#include "./ChunkMesh.h"
#include "./CubeFaces.h"
#include "./TileTypeManager.h"
#include "./Voxel.h"

class World;

class Chunk {
 public:
  Chunk(unsigned int x, unsigned int z);

  // Generate chunk voxels
  void generate(World& world, int seed);

  // Get block
  Voxel& get(unsigned int x, unsigned int y, unsigned int z);

  // Tessellate and such
  void update(World& world);

  // Render it all
  void render(World& world);

  // Position
  unsigned int getX() const { return index_x; }
  unsigned int getZ() const { return index_z; }

 private:
  unsigned int index_x;
  unsigned int index_z;

  Voxel blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]{};
  bool changed = false;

  unsigned int height_map[CHUNK_WIDTH][CHUNK_LENGTH]{};

  // Data
  ChunkMesh mesh;
};
