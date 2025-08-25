
#include "ChunkMesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "../utils/loaders.h"
#include "./World.h"

GLuint ChunkMesh::atlas = 0;

// Construct
ChunkMesh::ChunkMesh() {
  // Check atlas
  if (atlas == 0) {
    atlas = loaders::loadTexture("assets/images/textures/atlas.png");
  }

  // Make VAO, VBO, EBO
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
}

ChunkMesh::~ChunkMesh() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

// Fill array with given data
void ChunkMesh::fillFace(const FaceDefinition& face,
                         const glm::ivec3& base,
                         const glm::ivec3& worldPos,
                         GLuint atlasPos,
                         World& world) {
  const unsigned int atlasWidth = 8;
  const auto atlasX = static_cast<float>(atlasPos % atlasWidth);
  const auto atlasY = floorf(static_cast<float>(atlasPos) / atlasWidth);
  auto& chunks = world.getChunks();

  for (unsigned int i = 0; i < 6; i++) {
    // Get neighbours
    // Corner
    const glm::vec3 corner = 2.0f * face.vertices[i];

    // Sides
    glm::vec3 sideA = 2.0f * face.vertices[i];
    glm::vec3 sideB = 2.0f * face.vertices[i];

    // Move sides
    if (face.normal.x != 0.0F) {
      sideA.y -= corner.y;
      sideB.z -= corner.z;
    }

    if (face.normal.y != 0.0F) {
      sideA.x -= corner.x;
      sideB.z -= corner.z;
    }

    if (face.normal.z != 0.0F) {
      sideA.x -= corner.x;
      sideB.y -= corner.y;
    }

    // Sample neighbors adjacent to this face’s outside cell
    const bool solidSideA = chunks.isSolidAt(sideA + glm::vec3(worldPos));
    const bool solidSideB = chunks.isSolidAt(sideB + glm::vec3(worldPos));
    const bool solidCorner = chunks.isSolidAt(corner + glm::vec3(worldPos));

    // "Hard corner" rule: if both sides are filled, corner doesn't matter
    const int occ = (solidSideA && solidSideB)
                        ? 3
                        : (int)solidSideA + (int)solidSideB + (int)solidCorner;

    // Map 0..3 -> 1.0, 0.66, 0.33, 0.0 (tune if you want it less strong)
    const float ao = (3.0f - occ) / 3.0f;

    // Write vertex: position, normal, uv (atlas), AO
    const glm::vec3 pos = face.vertices[i] + glm::vec3(base);

    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);

    vertices.push_back(face.normal.x);
    vertices.push_back(face.normal.y);
    vertices.push_back(face.normal.z);

    vertices.push_back((faceUVs[i].x + atlasX) / atlasWidth);
    vertices.push_back((faceUVs[i].y + atlasY) / atlasWidth);

    if (asw::input::isKeyDown(asw::input::Key::F)) {
      vertices.push_back(1.0f);  // Disable AO when F is held
    } else {
      vertices.push_back(ao);
    }

    indices.push_back(indices.size());
  }
}

// Tessellate chunk
void ChunkMesh::tessellate(
    World& world,
    glm::ivec3 position,
    Voxel (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]) {
  auto& chunks = world.getChunks();

  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
      for (unsigned int k = 0; k < CHUNK_LENGTH; k++) {
        auto* parent = blk[i][t][k].getTile();
        auto type = parent->getType();

        // Empty block?
        if (type == TileID::Air) {
          continue;
        }

        const auto& atlasIds = parent->getAtlasIds();
        const glm::ivec3 base = glm::ivec3(i, t, k);
        const glm::ivec3 wPos = position + base;

        VoxelNeighbours neighbours{};
        neighbours.top =
            chunks.isSolidAt(glm::vec3(wPos.x, wPos.y + 1, wPos.z));
        neighbours.bottom =
            chunks.isSolidAt(glm::vec3(wPos.x, wPos.y - 1, wPos.z));
        neighbours.left =
            chunks.isSolidAt(glm::vec3(wPos.x - 1, wPos.y, wPos.z));
        neighbours.right =
            chunks.isSolidAt(glm::vec3(wPos.x + 1, wPos.y, wPos.z));
        neighbours.front =
            chunks.isSolidAt(glm::vec3(wPos.x, wPos.y, wPos.z + 1));
        neighbours.back =
            chunks.isSolidAt(glm::vec3(wPos.x, wPos.y, wPos.z - 1));

        if (!neighbours.top) {
          fillFace(topFace, base, wPos, atlasIds.top, world);
        }
        if (!neighbours.bottom) {
          fillFace(bottomFace, base, wPos, atlasIds.bottom, world);
        }
        if (!neighbours.left) {
          fillFace(leftFace, base, wPos, atlasIds.left, world);
        }
        if (!neighbours.right) {
          fillFace(rightFace, base, wPos, atlasIds.right, world);
        }
        if (!neighbours.front) {
          fillFace(frontFace, base, wPos, atlasIds.front, world);
        }
        if (!neighbours.back) {
          fillFace(backFace, base, wPos, atlasIds.back, world);
        }
      }
    }
  }

  glBindVertexArray(vao);

  if (vertices.empty() || indices.empty()) {
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
               &indices[0], GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                        (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // Texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                        (void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  // AO coord attribute
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                        (void*)(8 * sizeof(GLfloat)));
  glEnableVertexAttribArray(3);

  // Clear mesh
  numIndices = indices.size();
  vertices.clear();
  indices.clear();
}

void ChunkMesh::render(World& world,
                       unsigned int offsetX,
                       unsigned int offsetY,
                       unsigned int offsetZ) {
  auto& defaultShader = world.getGpuProgramManager().getShader("default");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, atlas);

  glm::mat4 model = glm::translate(
      glm::mat4(1.0f), glm::vec3(offsetX * CHUNK_WIDTH, offsetY * CHUNK_HEIGHT,
                                 offsetZ * CHUNK_LENGTH));

  defaultShader.setMat4("model", model);

  // Render
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
