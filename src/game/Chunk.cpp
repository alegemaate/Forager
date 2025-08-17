/*
  Chunk
  Allan Legemaate
  08/01/16
  Block of blocks!
*/

#include "Chunk.h"

#include "../constants/ids.h"
#include "TileTypeManager.h"

#include <iostream>

SimplexNoise Chunk::noise(0.02f, 0.02f, 2.0f, 0.47f);

// Construct
Chunk::Chunk(unsigned int x, unsigned int y, unsigned int z)
    : index_x(x), index_y(y), index_z(z) {
  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
      for (unsigned int u = 0; u < CHUNK_LENGTH; u++) {
        blk[i][t][u].setType(TileTypeManager::getTileByType(TILE_AIR));
      }
    }
  }

  // Init VAO
  mesh.tessellate(blk);
}

void Chunk::generate(int seed) {
  // STEP 1:
  // 2D Heightmap generation
  SimplexNoise heightMap = SimplexNoise(0.002f, 0.002f, 2.0f, 0.47f);

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
          blk[i][t][u].setType(TileTypeManager::getTileByType(TILE_AIR));
        }

        else if (t + 1 > height) {  // Grass
          blk[i][t][u].setType(TileTypeManager::getTileByType(TILE_GRASS));
        } else if (t + 4 > height) {  // Dirt
          blk[i][t][u].setType(TileTypeManager::getTileByType(TILE_DIRT));
        } else {  // Stone
          blk[i][t][u].setType(TileTypeManager::getTileByType(TILE_STONE));
        }
      }
    }
  }

  // // STEP 2:
  // // Fill with dirt
  // const auto chunkXOffset = seed + (index_x * CHUNK_WIDTH);
  // const auto chunkZOffset = seed + (index_z * CHUNK_LENGTH);
  // const auto chunkYOffset = seed + (index_y * CHUNK_HEIGHT);

  // for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
  //   auto noiseX = static_cast<float>(i + chunkXOffset);

  //   for (unsigned int u = 0; u < CHUNK_LENGTH; u++) {
  //     auto noiseZ = static_cast<float>(u + chunkZOffset);

  //     for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
  //       auto noiseY = static_cast<float>(t + chunkYOffset);

  //       auto val = Chunk::noise.fractal(10, noiseX, noiseZ, noiseY);

  //       if (val < 0.0f) {
  //         blk[i][t][u].setType(TileTypeManager::getTileByType(TILE_STONE));
  //       }
  //     }
  //   }
  // }

  mesh.tessellate(blk);
}

Voxel& Chunk::get(unsigned int x, unsigned int y, unsigned int z) {
  return blk[x][y][z];
}

void Chunk::set(unsigned int x,
                unsigned int y,
                unsigned int z,
                unsigned char type) {
  blk[x][y][z].setType(TileTypeManager::getTileByType(type));
  changed = true;
}

void Chunk::update() {
  if (changed) {
    mesh.tessellate(blk);
  }

  changed = false;
}

void Chunk::render() {
  mesh.render(index_x, index_y, index_z);
}
