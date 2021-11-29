
#include "ChunkMesh.h"

#include "../constants/ids.h"
#include "../core/SimplexNoise.h"
#include "../utils/loaders.h"
#include "TileTypeManager.h"

GLuint ChunkMesh::atlas = 0;

// Construct
ChunkMesh::ChunkMesh() {
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
}

// Fill array with given data
void ChunkMesh::fillFace(FaceDefenition face[6],
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
void ChunkMesh::tessellate(
    Voxel* blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]) {
  // Vertex counter
  numIndices = 0;

  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
      for (unsigned int k = 0; k < CHUNK_LENGTH; k++) {
        auto* parent = blk[i][t][k]->getTile();
        auto type = parent->getType();
        auto atlasIds = parent->getAtlasIds();

        // Empty block?
        if (!type) {
          continue;
        }

        // LEFT (-x)
        if (i == 0 || blk[i - 1][t][k]->getType() == 0) {
          fillFace(leftFace, glm::vec3(i, t, k), atlasIds.left);
        }

        // RIGHT (+x)
        if (i == CHUNK_WIDTH - 1 || blk[i + 1][t][k]->getType() == 0) {
          fillFace(rightFace, glm::vec3(i, t, k), atlasIds.right);
        }

        // BOTTOM (-y)
        if (t == 0 || blk[i][t - 1][k]->getType() == 0) {
          fillFace(bottomFace, glm::vec3(i, t, k), atlasIds.bottom);
        }

        // TOP (+y)
        if (t == CHUNK_HEIGHT - 1 || blk[i][t + 1][k]->getType() == 0) {
          fillFace(topFace, glm::vec3(i, t, k), atlasIds.top);
        }

        // BACK(-z)
        if (k == 0 || blk[i][t][k - 1]->getType() == 0) {
          fillFace(backFace, glm::vec3(i, t, k), atlasIds.back);
        }

        // FRONT (+z)
        if (k == CHUNK_LENGTH - 1 || blk[i][t][k + 1]->getType() == 0) {
          fillFace(frontFace, glm::vec3(i, t, k), atlasIds.front);
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

void ChunkMesh::render(unsigned int offsetX, unsigned int offsetZ) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, atlas);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(
      model, glm::vec3(offsetX * CHUNK_WIDTH, 0.0f, offsetZ * CHUNK_LENGTH));
  defaultShader->setMat4("model", model);

  // Render
  glBindVertexArray(chunkVAO);
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}
