
#ifndef FORAGER_CHUNK_MESH_H
#define FORAGER_CHUNK_MESH_H

#include <array>
#include <vector>

#include "../constants/globals.h"
#include "CubeFaces.h"
#include "Voxel.h"

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
  void fillFace(const std::array<FaceDefenition, 6>& face,
                const glm::vec3& offset,
                GLuint atlasPos,
                const VoxelNeighbours& neighbours);

  unsigned int vertexAO(bool side1, bool side2, bool corner);

  // Tessellate chunk
  void tessellate(Voxel (&blk)[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]);

  // Render it all
  void render(unsigned int offsetX, unsigned int offsetY, unsigned int offsetZ);

 private:
  GLuint chunkVAO{0};
  GLuint chunkVBO{0};
  GLuint chunkEBO{0};

  GLuint numIndices{0};

  std::vector<GLfloat> vertices;
  std::vector<GLuint> indices;

  static GLuint atlas;  // Texture atlas
};

#endif  // FORAGER_CHUNK_MESH_H
