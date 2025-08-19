/*
  Chunk
  Allan Legemaate
  08/01/16
  Block of blocks!
*/

#include "Chunk.h"

#include "./TileTypeManager.h"
#include "./World.h"

// Construct
Chunk::Chunk(unsigned int x, unsigned int y, unsigned int z)
    : index_x(x), index_y(y), index_z(z) {}

void Chunk::generate(TileTypeManager& tileManager, int seed) {
  // STEP 1:
  // 2D Heightmap generation
  const SimplexNoise heightMap = SimplexNoise(0.002f, 0.002f, 2.0f, 0.47f);

  for (unsigned int x = 0; x < CHUNK_WIDTH; x++) {
    auto noiseX = static_cast<float>(x + seed + (index_x * CHUNK_WIDTH));
    for (unsigned int z = 0; z < CHUNK_LENGTH; z++) {
      auto noiseZ = static_cast<float>(z + seed + (index_z * CHUNK_LENGTH));
      auto val = heightMap.fractal(10, noiseX, noiseZ);

      // Height of terrain at this (x,z) position
      // Cache for future steps
      auto height =
          static_cast<unsigned int>((val + 1.0f) * (CHUNK_HEIGHT - 1) / 2.0f);
      height_map[x][z] = height;

      for (unsigned int y = 0; y < CHUNK_HEIGHT; y++) {
        // Air
        if (y > height) {
          blk[x][y][z].setType(tileManager.getTileByType(TileID::Air));
        }

        else if (y + 1 > height) {  // Grass
          blk[x][y][z].setType(tileManager.getTileByType(TileID::Grass));
        } else if (y + 4 > height) {  // Dirt
          blk[x][y][z].setType(tileManager.getTileByType(TileID::Dirt));
        } else {  // Stone
          blk[x][y][z].setType(tileManager.getTileByType(TileID::Stone));
        }
      }
    }
  }

  // STEP 2:
  // Caves
  const SimplexNoise caveMap = SimplexNoise(0.03f, 0.03f, 2.0f, 0.47f);

  const auto chunkXOffset = seed + (index_x * CHUNK_WIDTH);
  const auto chunkZOffset = seed + (index_z * CHUNK_LENGTH);
  const auto chunkYOffset = seed + (index_y * CHUNK_HEIGHT);

  for (unsigned int x = 0; x < CHUNK_WIDTH; x++) {
    auto noiseX = static_cast<float>(x + chunkXOffset);

    for (unsigned int z = 0; z < CHUNK_LENGTH; z++) {
      auto noiseZ = static_cast<float>(z + chunkZOffset);
      auto height = height_map[x][z];

      for (unsigned int y = 4; y < height - 4; y++) {
        auto noiseY = static_cast<float>(y + chunkYOffset);
        auto val = caveMap.fractal(10, noiseX, noiseZ, noiseY);

        if (val > 0.0f) {
          blk[x][y][z].setType(tileManager.getTileByType(TileID::Air));
        }
      }
    }
  }

  mesh.tessellate(blk);
}

Voxel& Chunk::get(unsigned int x, unsigned int y, unsigned int z) {
  return blk[x][y][z];
}

void Chunk::update() {
  if (changed) {
    mesh.tessellate(blk);
  }

  changed = false;
}

void Chunk::render(World& world) {
  mesh.render(world, index_x, index_y, index_z);
}
