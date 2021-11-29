/*
  Chunk
  Allan Legemaate
  08/01/16
  Block of blocks!
*/

#include "Chunk.h"

#include "../constants/ids.h"
#include "../core/SimplexNoise.h"
#include "../utils/loaders.h"
#include "TileTypeManager.h"

// Construct
Chunk::Chunk(unsigned int x, unsigned int z) : index_x(x), index_z(z) {
  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
      for (unsigned int u = 0; u < CHUNK_LENGTH; u++) {
        blk[i][t][u] = new Voxel(glm::u8vec3(i, t, u),
                                 TileTypeManager::getTileByType(TILE_AIR));
      }
    }
  }

  // Init VAO
  mesh.tessellate(blk);
}

void Chunk::generate(int seed) {
  // Height
  auto* sn_h = new SimplexNoise(1.0f, 1.0f, 2.0f, 0.47f);

  // STEP 1:
  // Start with air
  for (auto& i : blk) {
    for (auto& t : i) {
      for (auto& u : t) {
        u->setType(TileTypeManager::getTileByType(TILE_AIR));
        u->setBiome(BIOME_NONE);
      }
    }
  }

  // STEP 2:
  // Fill with dirt
  const unsigned int HALF_HEIGHT = CHUNK_HEIGHT / 2.0f;

  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    auto noiseX = static_cast<float>(seed + i + index_x * CHUNK_WIDTH) / 500.0f;

    for (unsigned int u = 0; u < CHUNK_LENGTH; u++) {
      auto noiseZ =
          static_cast<float>(seed + u + index_z * CHUNK_LENGTH) / 500.0f;

      auto val = sn_h->fractal(10, noiseX, noiseZ);
      auto height = (val + 1.0f) * HALF_HEIGHT;

      for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
        if (static_cast<float>(t) < height) {
          blk[i][t][u]->setType(TileTypeManager::getTileByType(TILE_GRASS));
        }
      }
    }
  }

  changed = true;
}

Voxel* Chunk::get(unsigned int x, unsigned int y, unsigned int z) {
  return blk[x][y][z];
}

void Chunk::set(unsigned int x,
                unsigned int y,
                unsigned int z,
                unsigned char type) {
  blk[x][y][z]->setType(TileTypeManager::getTileByType(type));
  changed = true;
}

void Chunk::update() {
  if (changed) {
    mesh.tessellate(blk);
  }

  changed = false;
}

void Chunk::render() {
  mesh.render(index_x, index_z);
}
