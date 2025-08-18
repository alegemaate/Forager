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

  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    auto noiseX = static_cast<float>(i + seed + (index_x * CHUNK_WIDTH));
    for (unsigned int u = 0; u < CHUNK_LENGTH; u++) {
      auto noiseZ = static_cast<float>(u + seed + (index_z * CHUNK_LENGTH));
      auto val = heightMap.fractal(10, noiseX, noiseZ);
      auto height =
          static_cast<unsigned int>((val + 1.0f) * (CHUNK_HEIGHT - 1) / 2.0f);

      for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
        // Air
        if (t > height) {
          blk[i][t][u].setType(tileManager.getTileByType(TileID::Air));
        }

        else if (t + 1 > height) {  // Grass
          blk[i][t][u].setType(tileManager.getTileByType(TileID::Grass));
        } else if (t + 4 > height) {  // Dirt
          blk[i][t][u].setType(tileManager.getTileByType(TileID::Dirt));
        } else {  // Stone
          blk[i][t][u].setType(tileManager.getTileByType(TileID::Stone));
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

  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    auto noiseX = static_cast<float>(i + chunkXOffset);

    for (unsigned int u = 0; u < CHUNK_LENGTH; u++) {
      auto noiseZ = static_cast<float>(u + chunkZOffset);

      for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
        auto noiseY = static_cast<float>(t + chunkYOffset);

        auto val = caveMap.fractal(10, noiseX, noiseZ, noiseY);

        if (val > 0.0f) {
          blk[i][t][u].setType(tileManager.getTileByType(TileID::Air));
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
