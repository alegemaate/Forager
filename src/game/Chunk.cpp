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

GLuint Chunk::atlas = 0;

// Construct
Chunk::Chunk(int x, int z) : index_x(x), index_z(z) {
  // Check atlas
  if (atlas == 0) {
    atlas = loaders::loadTexture("assets/images/textures/atlas.png");
  }

  // 6 sides of cube * 6 indices per cube
  maxIndices = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LENGTH * 6 * 6;

  // 8 info per vertices (vec3 pos, vec3 norm, vec2 tex)
  vertices = new GLfloat[8 * maxIndices];
  indices = new GLuint[maxIndices];

  // Make vbo
  glGenVertexArrays(1, &chunkVAO);
  glGenBuffers(1, &chunkVBO);
  glGenBuffers(1, &chunkEBO);

  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
      for (unsigned int u = 0; u < CHUNK_LENGTH; u++) {
        blk[i][t][u] =
            new Tile(i, t, u, TileTypeManager::getTileByType(TILE_AIR));
      }
    }
  }

  // Init VAO
  tessellate();
}

// Fill array with given data
void Chunk::fillFace(FaceDefenition face[6],
                     glm::vec3 offset,
                     GLuint atlasPos) {
  const unsigned int atlasSize = 8;
  const auto atlasX = static_cast<float>(atlasPos % atlasSize);
  const auto atlasY = floorf(static_cast<float>(atlasPos) / atlasSize);

  for (unsigned int i = 0; i < 6; i++) {
    glm::vec3 pos = face[i].position + offset;

    const auto index = numIndices * 8;

    vertices[index] = pos.x;
    vertices[index + 1] = pos.y;
    vertices[index + 2] = pos.z;

    vertices[index + 3] = face[i].normal.x;
    vertices[index + 4] = face[i].normal.y;
    vertices[index + 5] = face[i].normal.z;

    vertices[index + 6] = (face[i].texture.x + atlasX) / atlasSize;
    vertices[index + 7] = (face[i].texture.y + atlasY) / atlasSize;

    indices[numIndices] = numIndices;
    numIndices++;
  }
}

// Tessellate chunk
void Chunk::tessellate() {
  // Vertex counter
  numIndices = 0;

  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
      for (unsigned int k = 0; k < CHUNK_LENGTH; k++) {
        auto type = blk[i][t][k]->getType();
        auto atlas = blk[i][t][k]->getTile()->getAtlasIds();

        // Empty block?
        if (!type) {
          continue;
        }

        // LEFT (-x)
        if (i == 0 || (i > 0 && blk[i - 1][t][k]->getType() == 0)) {
          fillFace(leftFace, glm::vec3(i, t, k), atlas.left);
        }

        // RIGHT (+x)
        if (i == CHUNK_WIDTH - 1 ||
            (i < CHUNK_WIDTH && blk[i + 1][t][k]->getType() == 0)) {
          fillFace(rightFace, glm::vec3(i, t, k), atlas.right);
        }

        // BOTTOM (-y)
        if (t == 0 || (t > 0 && blk[i][t - 1][k]->getType() == 0)) {
          fillFace(bottomFace, glm::vec3(i, t, k), atlas.bottom);
        }

        // TOP (+y)
        if (t == CHUNK_HEIGHT - 1 ||
            (t < CHUNK_HEIGHT && blk[i][t + 1][k]->getType() == 0)) {
          fillFace(topFace, glm::vec3(i, t, k), atlas.top);
        }

        // BACK(-z)
        if (k == 0 || (k > 0 && blk[i][t][k - 1]->getType() == 0)) {
          fillFace(backFace, glm::vec3(i, t, k), atlas.back);
        }

        // FRONT (+z)
        if (k == CHUNK_LENGTH - 1 ||
            (k < CHUNK_LENGTH && blk[i][t][k + 1]->getType() == 0)) {
          fillFace(frontFace, glm::vec3(i, t, k), atlas.front);
        }
      }
    }
  }

  glBindVertexArray(chunkVAO);

  glBindBuffer(GL_ARRAY_BUFFER, chunkVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * numIndices, vertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices, indices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
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

Tile* Chunk::get(unsigned int x, unsigned int y, unsigned int z) {
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
    tessellate();
  }

  changed = false;
}

void Chunk::render() {
  // Tessellate when modified
  if (changed) {
    update();
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, atlas);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(
      model, glm::vec3(index_x * CHUNK_WIDTH, 0.0f, index_z * CHUNK_LENGTH));
  defaultShader->setMat4("model", model);

  // Render
  glBindVertexArray(chunkVAO);
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}
