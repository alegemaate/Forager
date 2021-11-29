
#ifndef FORAGER_CHUNK_MESH_H
#define FORAGER_CHUNK_MESH_H

#include "../constants/globals.h"
#include "CubeFaces.h"
#include "Voxel.h"

class ChunkMesh {
 public:
  ChunkMesh();

  // Fill a given face
  void fillFace(FaceDefenition face[6], glm::vec3 offset, GLuint atlasPos);

  // Tessellate chunk
  void tessellate(Voxel* blk[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH]);

  // Render it all
  void render(unsigned int offsetX, unsigned int offsetZ);

 private:
  // Data
  GLuint chunkVAO = 0;
  GLuint chunkVBO = 0;
  GLuint chunkEBO = 0;

  GLuint maxIndices = 0;
  GLuint numIndices = 0;

  GLfloat* vertices;
  GLuint* indices;

  static GLuint atlas;
};

#endif  // FORAGER_CHUNK_MESH_H
