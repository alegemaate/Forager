#include "CubeFaces.h"

FaceDefenition leftFace[6] = {
    {glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 0)},
    {glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 0)},
    {glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 1)},
    {glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 0)},
    {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 1)},
    {glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 1)}};

FaceDefenition rightFace[6] = {
    {glm::vec3(0.5, 0.5, 0.5), glm::vec3(1, 0, 0), glm::vec2(0, 0)},
    {glm::vec3(0.5, -0.5, 0.5), glm::vec3(1, 0, 0), glm::vec2(1, 0)},
    {glm::vec3(0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 1)},
    {glm::vec3(0.5, 0.5, 0.5), glm::vec3(1, 0, 0), glm::vec2(0, 0)},
    {glm::vec3(0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 1)},
    {glm::vec3(0.5, 0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(0, 1)},
};

FaceDefenition bottomFace[6] = {
    {glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, -1, 0), glm::vec2(0, 0)},
    {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1, 0)},
    {glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1, 1)},
    {glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, -1, 0), glm::vec2(0, 0)},
    {glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1, 1)},
    {glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, -1, 0), glm::vec2(0, 1)},
};

FaceDefenition topFace[6] = {
    {glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0)},
    {glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0)},
    {glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1)},
    {glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0)},
    {glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1)},
    {glm::vec3(0.5, 0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1)},
};

FaceDefenition backFace[6] = {
    {glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 0)},
    {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 0)},
    {glm::vec3(0.5, 0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 1)},
    {glm::vec3(0.5, 0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 0)},
    {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 1)},
    {glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 1)},
};

FaceDefenition frontFace[6] = {
    {glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0)},
    {glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(1, 0)},
    {glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(1, 1)},
    {glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0)},
    {glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(1, 1)},
    {glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(0, 1)},
};