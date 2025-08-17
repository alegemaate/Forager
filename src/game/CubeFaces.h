#pragma once

#include <array>
#include <glm/glm.hpp>

struct FaceDefenition {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture;
};

extern std::array<FaceDefenition, 6> leftFace;
extern std::array<FaceDefenition, 6> rightFace;
extern std::array<FaceDefenition, 6> topFace;
extern std::array<FaceDefenition, 6> bottomFace;
extern std::array<FaceDefenition, 6> frontFace;
extern std::array<FaceDefenition, 6> backFace;
