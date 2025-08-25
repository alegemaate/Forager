
#pragma once

#include <GL/glew.h>
#include <array>
#include <functional>
#include <vector>

#include "CubeFaces.h"
#include "Voxel.h"

constexpr size_t CHUNK_WIDTH = 16;
constexpr size_t CHUNK_HEIGHT = 128;
constexpr size_t CHUNK_LENGTH = 16;

class World;

struct VoxelNeighbours {
  bool top;
  bool bottom;
  bool left;
  bool right;
  bool front;
  bool back;
};

class ChunkMesh {
 public:
  ChunkMesh();
  ~ChunkMesh();

  // Fill a given face
  void fillFace(const FaceDefinition& face,
                const glm::ivec3& base,
                const glm::ivec3& worldPos,
                GLuint atlasPos,
                World& world);

  // Tessellate chunk
  void tessellate(World& world,
                  glm::ivec3 position,
                  Voxel (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]);

  // Render it all
  void render(World& world,
              unsigned int offsetX,
              unsigned int offsetY,
              unsigned int offsetZ);

 private:
  GLuint vao{0};
  GLuint vbo{0};
  GLuint ebo{0};

  GLuint numIndices{0};

  std::vector<GLfloat> vertices;
  std::vector<GLuint> indices;

  static GLuint atlas;  // Texture atlas
};
