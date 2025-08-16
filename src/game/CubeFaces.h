#ifndef FORAGER_CUBEFACES_H
#define FORAGER_CUBEFACES_H

#include <glm/glm.hpp>

struct FaceDefenition {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture;
};

extern FaceDefenition leftFace[6];
extern FaceDefenition rightFace[6];
extern FaceDefenition topFace[6];
extern FaceDefenition bottomFace[6];
extern FaceDefenition frontFace[6];
extern FaceDefenition backFace[6];

#endif  // FORAGER_CUBEFACES_H
