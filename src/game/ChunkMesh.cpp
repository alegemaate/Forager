
#include "ChunkMesh.h"

#include "../utils/loaders.h"

GLuint ChunkMesh::atlas = 0;

static inline bool isSolidInChunk(
    int x,
    int y,
    int z,
    Voxel (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]) {
  if (x < 0 || y < 0 || z < 0 || x >= (int)CHUNK_WIDTH ||
      y >= (int)CHUNK_HEIGHT || z >= (int)CHUNK_LENGTH) {
    return false;  // treat out-of-chunk as air
  }
  return blk[x][y][z].getType() != TileID::Air;
}

static inline float aoFrom3(bool side1, bool side2, bool corner) {
  // "Hard corner" rule: if both sides are filled, corner doesn't matter
  const int occ = (side1 && side2) ? 3 : (int)side1 + (int)side2 + (int)corner;
  // Map 0..3 -> 1.0, 0.66, 0.33, 0.0 (tune if you want it less strong)
  return (3 - occ) / 3.0f;
}

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
void ChunkMesh::fillFace(const std::array<FaceDefenition, 6>& face,
                         const glm::ivec3& base,
                         GLuint atlasPos,
                         const std::function<bool(int, int, int)>& solid) {
  const unsigned int atlasSize = 8;
  const auto atlasX = static_cast<float>(atlasPos % atlasSize);
  const auto atlasY = floorf(static_cast<float>(atlasPos) / atlasSize);

  // Determine the face’s major axis from its normal (all 6 verts share it)
  const glm::vec3 n = face[0].normal;
  const int major = (fabsf(n.x) > 0.5f) ? 0 : ((fabsf(n.y) > 0.5f) ? 1 : 2);
  const int aAxis = (major + 1) % 3;
  const int bAxis = (major + 2) % 3;
  const int nSign = ((&n.x)[major] > 0.f) ? +1 : -1;

  // Integer vectors for offset along major/tangent axes
  auto ivec = [](int ax, int s) {
    glm::ivec3 v(0);
    (&v.x)[ax] = s;
    return v;
  };
  const glm::ivec3 nVec = ivec(major, nSign);

  for (unsigned int i = 0; i < 6; i++) {
    // Local cube-space vertex (should be 0/1 for each coord in your
    // FaceDefenition)
    const glm::vec3 pLocal = face[i].position;

    // Choose side directions along the two tangents based on whether the
    // vertex is min/max on that axis
    const int sA = ((&pLocal.x)[aAxis] > 0.5f) ? +1 : -1;
    const int sB = ((&pLocal.x)[bAxis] > 0.5f) ? +1 : -1;

    const glm::ivec3 tA = ivec(aAxis, sA);
    const glm::ivec3 tB = ivec(bAxis, sB);

    // Sample neighbors adjacent to *this face’s* outside cell
    const glm::ivec3 pFace = base + nVec;  // cell just outside the face
    const bool side1 = solid(pFace.x + tA.x, pFace.y + tA.y, pFace.z + tA.z);
    const bool side2 = solid(pFace.x + tB.x, pFace.y + tB.y, pFace.z + tB.z);
    const bool corner = solid(pFace.x + tA.x + tB.x, pFace.y + tA.y + tB.y,
                              pFace.z + tA.z + tB.z);

    const float ao = aoFrom3(side1, side2, corner);

    // Write vertex: position, normal, uv (atlas), AO
    const glm::vec3 pos = pLocal + glm::vec3(base);

    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);

    vertices.push_back(face[i].normal.x);
    vertices.push_back(face[i].normal.y);
    vertices.push_back(face[i].normal.z);

    vertices.push_back((face[i].texture.x + atlasX) / atlasSize);
    vertices.push_back((face[i].texture.y + atlasY) / atlasSize);

    vertices.push_back(ao);

    indices.push_back(indices.size());
  }
}

// Tessellate chunk
void ChunkMesh::tessellate(
    Voxel (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]) {
  for (unsigned int i = 0; i < CHUNK_WIDTH; i++) {
    for (unsigned int t = 0; t < CHUNK_HEIGHT; t++) {
      for (unsigned int k = 0; k < CHUNK_LENGTH; k++) {
        auto* parent = blk[i][t][k].getTile();
        auto type = parent->getType();

        // Empty block?
        if (type == TileID::Air) {
          continue;
        }

        auto& atlasIds = parent->getAtlasIds();
        glm::ivec3 base(i, t, k);

        auto solid = [&](int x, int y, int z) {
          return isSolidInChunk(x, y, z, blk);
        };

        VoxelNeighbours neighbours{};
        neighbours.top =
            t != CHUNK_HEIGHT - 1 && blk[i][t + 1][k].getType() != TileID::Air;
        neighbours.bottom = t != 0 && blk[i][t - 1][k].getType() != TileID::Air;
        neighbours.left = i != 0 && blk[i - 1][t][k].getType() != TileID::Air;
        neighbours.right =
            i != CHUNK_WIDTH - 1 && blk[i + 1][t][k].getType() != TileID::Air;
        neighbours.front =
            k != CHUNK_LENGTH - 1 && blk[i][t][k + 1].getType() != TileID::Air;
        neighbours.back = k != 0 && blk[i][t][k - 1].getType() != TileID::Air;

        if (!neighbours.top) {
          fillFace(topFace, base, atlasIds.top, solid);
        }
        if (!neighbours.bottom) {
          fillFace(bottomFace, base, atlasIds.bottom, solid);
        }
        if (!neighbours.left) {
          fillFace(leftFace, base, atlasIds.left, solid);
        }
        if (!neighbours.right) {
          fillFace(rightFace, base, atlasIds.right, solid);
        }
        if (!neighbours.front) {
          fillFace(frontFace, base, atlasIds.front, solid);
        }
        if (!neighbours.back) {
          fillFace(backFace, base, atlasIds.back, solid);
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

void ChunkMesh::render(unsigned int offsetX,
                       unsigned int offsetY,
                       unsigned int offsetZ) {
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
