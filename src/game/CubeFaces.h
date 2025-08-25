#pragma once

#include <array>
#include <glm/glm.hpp>

struct FaceDefinition {
  glm::vec3 center;
  glm::vec3 normal;
  std::array<glm::vec3, 6> vertices;
};

extern std::array<glm::vec2, 6> faceUVs;

extern FaceDefinition leftFace;
extern FaceDefinition rightFace;
extern FaceDefinition topFace;
extern FaceDefinition bottomFace;
extern FaceDefinition frontFace;
extern FaceDefinition backFace;
