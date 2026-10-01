#pragma once

#include <array>
#include <glm/glm.hpp>

struct FaceDefinition
{
    glm::vec3                normal;
    std::array<glm::vec3, 4> vertices;
    std::array<glm::vec3, 8> neighbours;
};

extern std::array<glm::vec2, 4> faceUVs;

extern FaceDefinition leftFace;
extern FaceDefinition rightFace;
extern FaceDefinition topFace;
extern FaceDefinition bottomFace;
extern FaceDefinition frontFace;
extern FaceDefinition backFace;
