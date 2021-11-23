/*
  Chunk
  Allan Legemaate
  08/01/16
  Block of blocks!
*/

#include "Chunk.h"

#include "../core/SimplexNoise.h"
#include "TileTypeManager.h"

// Construct
Chunk::Chunk(int x, int z) : index_x(x), index_z(z) {
  // Stores vertices
  num_indices = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LENGTH * 6 * 6;
  geometry = new GLfloat[9 * num_indices];
  indices = new unsigned long[num_indices];

  // Make vbo
  glGenBuffers(1, &geometry_array);
  glGenBuffers(1, &indices_array);

  for (int i = 0; i < CHUNK_WIDTH; i++) {
    for (int t = 0; t < CHUNK_HEIGHT; t++) {
      for (int u = 0; u < CHUNK_LENGTH; u++) {
        blk[i][t][u] =
            new Tile(i, t, u, TileTypeManager::getTileByType(TILE_AIR));
      }
    }
  }
}

// Fill array with given data
void Chunk::fillArray(glm::vec3 posVec,
                      glm::vec3 normVec,
                      glm::vec2 texVec,
                      GLfloat* arr,
                      unsigned long index) {
  arr[index] = posVec.x;
  arr[index + 1] = posVec.y;
  arr[index + 2] = posVec.z;

  arr[index + 3] = normVec.x;
  arr[index + 4] = normVec.y;
  arr[index + 5] = normVec.z;

  arr[index + 6] = texVec.x;
  arr[index + 7] = texVec.y;
}

// Tessellate chunk
void Chunk::tessellate() {
  // Vertex counter
  unsigned long j = 0;

  for (int i = 0; i < CHUNK_WIDTH; i++) {
    for (int t = 0; t < CHUNK_HEIGHT; t++) {
      for (int k = 0; k < CHUNK_LENGTH; k++) {
        auto type = blk[i][t][k]->getType();

        // Empty block?
        if (!type) {
          continue;
        }

        // LEFT (-x)
        if (i == 0 || (i > 0 && blk[i - 1][t][k]->getType() == 0)) {
          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k + 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k - 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(-1, 0, 0),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // RIGHT (+x)
        if (i == CHUNK_WIDTH - 1 ||
            (i < CHUNK_WIDTH && blk[i + 1][t][k]->getType() == 0)) {
          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;
          fillArray(glm::vec3(i + 0.5, t - 0.5, k + 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k - 0.5), glm::vec3(1, 0, 0),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // BOTTOM (-y)
        if (t == 0 || (t > 0 && blk[i][t - 1][k]->getType() == 0)) {
          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k - 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k + 0.5), glm::vec3(0, -1, 0),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // TOP (+y)
        if (t == CHUNK_HEIGHT - 1 ||
            (t < CHUNK_HEIGHT && blk[i][t + 1][k]->getType() == 0)) {
          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k + 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k - 0.5), glm::vec3(0, 1, 0),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // BACK(-z)
        if (k == 0 || (k > 0 && blk[i][t][k - 1]->getType() == 0)) {
          fillArray(glm::vec3(i + 0.5, t - 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k - 0.5), glm::vec3(0, 0, -1),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }

        // FRONT (+z)
        if (k == CHUNK_LENGTH - 1 ||
            (k < CHUNK_LENGTH && blk[i][t][k + 1]->getType() == 0)) {
          fillArray(glm::vec3(i - 0.5, t + 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t - 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(1, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i - 0.5, t + 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(0, 0), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t - 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(1, 1), geometry, j * 9);
          indices[j] = j;
          j++;

          fillArray(glm::vec3(i + 0.5, t + 0.5, k + 0.5), glm::vec3(0, 0, 1),
                    glm::vec2(0, 1), geometry, j * 9);
          indices[j] = j;
          j++;
        }
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, geometry_array);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * j, nullptr,
               GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 9 * j, geometry);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_array);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned long) * num_indices,
               nullptr, GL_STATIC_DRAW);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                  sizeof(unsigned long) * num_indices, indices);
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
        u->jiggle(0, 0, 0);
      }
    }
  }

  // STEP 2:
  // Fill with dirt
  for (int i = 0; i < CHUNK_WIDTH; i++) {
    for (int u = 0; u < CHUNK_LENGTH; u++) {
      auto noiseX =
          static_cast<float>(seed + i + index_x * CHUNK_WIDTH) / 500.0f;
      auto noiseZ =
          static_cast<float>(seed + u + index_z * CHUNK_LENGTH) / 500.0f;
      auto val = sn_h->fractal(10, noiseX, noiseZ);
      auto height = (val + 1.0f) * (CHUNK_HEIGHT / 2.0f);

      for (int t = 0; t < CHUNK_HEIGHT; t++) {
        if (static_cast<float>(t) < height) {
          blk[i][t][u]->setType(TileTypeManager::getTileByType(TILE_GRASS));
        }
      }
    }
  }

  changed = true;
}

Tile* Chunk::get(int x, int y, int z) {
  return blk[x][y][z];
}

void Chunk::set(int x, int y, int z, unsigned char type) {
  blk[x][y][z]->setType(TileTypeManager::getTileByType(type));
  changed = true;
}

void Chunk::update() {
  if (changed) {
    tessellate();
  }

  changed = false;
}

void Chunk::render() {
  // Tessellate when modified
  if (changed) {
    update();
  }

  // Go into model view matrix
  glPushMatrix();

  // Translate in
  glTranslatef(index_x * CHUNK_WIDTH, 0.0f, index_z * CHUNK_LENGTH);

  // Render
  // Step 1
  glBindBuffer(GL_ARRAY_BUFFER, geometry_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_array);

  // Step 2
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  // Step 3
  glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 9, (float*)(sizeof(GLfloat) * 3));
  glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 9, nullptr);
  glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 9,
                    (float*)(sizeof(GLfloat) * 6));

  glBindTexture(GL_TEXTURE_2D, 8);

  //  Step 4
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);

  //   Step 5
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glPopMatrix();
}
