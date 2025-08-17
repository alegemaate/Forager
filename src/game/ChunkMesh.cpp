
#include "ChunkMesh.h"

#include "../utils/loaders.h"

#include <iostream>

GLuint ChunkMesh::atlas = 0;

// Construct
ChunkMesh::ChunkMesh() {
  // Check atlas
  if (atlas == 0) {
    atlas = loaders::loadTexture("assets/images/textures/atlas.png");
  }

  // Make vbo
  glGenVertexArrays(1, &chunkVAO);
  glGenBuffers(1, &chunkVBO);
  glGenBuffers(1, &chunkEBO);
}

ChunkMesh::~ChunkMesh() {
  glDeleteVertexArrays(1, &chunkVAO);
  glDeleteBuffers(1, &chunkVBO);
  glDeleteBuffers(1, &chunkEBO);
}

// Fill array with given data
void ChunkMesh::fillFace(const std::array<FaceDefenition, 6>& face,
                         const glm::vec3& offset,
                         GLuint atlasPos,
                         const VoxelNeighbours& neighbours) {
  const unsigned int atlasSize = 8;
  const auto atlasX = static_cast<float>(atlasPos % atlasSize);
  const auto atlasY = floorf(static_cast<float>(atlasPos) / atlasSize);

  for (unsigned int i = 0; i < 6; i++) {
    const glm::vec3 pos = face[i].position + offset;

    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);

    vertices.push_back(face[i].normal.x);
    vertices.push_back(face[i].normal.y);
    vertices.push_back(face[i].normal.z);

    vertices.push_back((face[i].texture.x + atlasX) / atlasSize);
    vertices.push_back((face[i].texture.y + atlasY) / atlasSize);

    vertices.push_back(0);

    indices.push_back(indices.size());
  }
}

unsigned int ChunkMesh::vertexAO(bool side1, bool side2, bool corner) {
  if (side1 && side2) {
    return 0;
  }

  return 3 - (side1 + side2 + corner);
}

// Tessellate chunk
void ChunkMesh::tessellate(
    Voxel (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]) {
  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
      for (unsigned int k = 0; k < CHUNK_LENGTH; k++) {
        auto* parent = blk[i][t][k].getTile();
        auto type = parent->getType();
        auto atlasIds = parent->getAtlasIds();

        // Empty block?
        if (!type) {
          continue;
        }

        VoxelNeighbours neighbours{};
        neighbours.top =
            t != CHUNK_HEIGHT - 1 && blk[i][t + 1][k].getType() != 0;
        neighbours.bottom = t != 0 && blk[i][t - 1][k].getType() != 0;
        neighbours.left = i != 0 && blk[i - 1][t][k].getType() != 0;
        neighbours.right =
            i != CHUNK_WIDTH - 1 && blk[i + 1][t][k].getType() != 0;
        neighbours.front =
            k != CHUNK_LENGTH - 1 && blk[i][t][k + 1].getType() != 0;
        neighbours.back = k != 0 && blk[i][t][k - 1].getType() != 0;

        if (!neighbours.top) {
          fillFace(topFace, glm::vec3(i, t, k), atlasIds.top, neighbours);
        }

        if (!neighbours.bottom) {
          fillFace(bottomFace, glm::vec3(i, t, k), atlasIds.bottom, neighbours);
        }

        if (!neighbours.left) {
          fillFace(leftFace, glm::vec3(i, t, k), atlasIds.left, neighbours);
        }

        if (!neighbours.right) {
          fillFace(rightFace, glm::vec3(i, t, k), atlasIds.right, neighbours);
        }

        if (!neighbours.front) {
          fillFace(frontFace, glm::vec3(i, t, k), atlasIds.front, neighbours);
        }

        if (!neighbours.back) {
          fillFace(backFace, glm::vec3(i, t, k), atlasIds.back, neighbours);
        }
      }
    }
  }

  glBindVertexArray(chunkVAO);

  if (vertices.empty() || indices.empty()) {
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, chunkVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
               &indices[0], GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                        (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                        (void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  // ao coord attribute
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                        (void*)(8 * sizeof(GLfloat)));
  glEnableVertexAttribArray(3);

  // Clear mesh
  numIndices = indices.size();
  vertices.clear();
  indices.clear();
}

void ChunkMesh::render(unsigned int offsetX,
                       unsigned int offsetY,
                       unsigned int offsetZ) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, atlas);

  glm::mat4 model = glm::translate(
      glm::mat4(1.0f), glm::vec3(offsetX * CHUNK_WIDTH, offsetY * CHUNK_HEIGHT,
                                 offsetZ * CHUNK_LENGTH));

  defaultShader->setMat4("model", model);

  // Render
  glBindVertexArray(chunkVAO);
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
