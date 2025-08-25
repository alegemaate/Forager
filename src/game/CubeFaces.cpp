#include "CubeFaces.h"

static constexpr float H = 0.5f;

// Helper: same UVs for all faces
std::array<glm::vec2, 6> faceUVs = {glm::vec2{0.f, 1.f}, glm::vec2{1.f, 1.f},
                                    glm::vec2{1.f, 0.f}, glm::vec2{0.f, 1.f},
                                    glm::vec2{1.f, 0.f}, glm::vec2{0.f, 0.f}};

FaceDefinition frontFace = {
    .center = glm::vec3(0, 0, H),
    .normal = glm::vec3(0, 0, 1),
    .vertices =
        {
            glm::vec3(-H, -H, H),
            glm::vec3(H, -H, H),
            glm::vec3(H, H, H),
            glm::vec3(-H, -H, H),
            glm::vec3(H, H, H),
            glm::vec3(-H, H, H),
        },
};

FaceDefinition backFace = {
    .center = glm::vec3(0, 0, -H),
    .normal = glm::vec3(0, 0, -1),
    .vertices =
        {
            glm::vec3(H, -H, -H),
            glm::vec3(-H, -H, -H),
            glm::vec3(-H, H, -H),
            glm::vec3(H, -H, -H),
            glm::vec3(-H, H, -H),
            glm::vec3(H, H, -H),
        },
};

FaceDefinition leftFace = {
    .center = glm::vec3(-H, 0, 0),
    .normal = glm::vec3(-1, 0, 0),
    .vertices =
        {
            glm::vec3(-H, -H, -H),
            glm::vec3(-H, -H, H),
            glm::vec3(-H, H, H),
            glm::vec3(-H, -H, -H),
            glm::vec3(-H, H, H),
            glm::vec3(-H, H, -H),
        },
};

FaceDefinition rightFace = {
    .center = glm::vec3(H, 0, 0),
    .normal = glm::vec3(1, 0, 0),
    .vertices =
        {
            glm::vec3(H, -H, H),
            glm::vec3(H, -H, -H),
            glm::vec3(H, H, -H),
            glm::vec3(H, -H, H),
            glm::vec3(H, H, -H),
            glm::vec3(H, H, H),
        },
};

FaceDefinition topFace = {
    .center = glm::vec3(0, H, 0),
    .normal = glm::vec3(0, 1, 0),
    .vertices =
        {
            glm::vec3(-H, H, H),
            glm::vec3(H, H, H),
            glm::vec3(H, H, -H),
            glm::vec3(-H, H, H),
            glm::vec3(H, H, -H),
            glm::vec3(-H, H, -H),
        },
};

FaceDefinition bottomFace = {
    .center = glm::vec3(0, -H, 0),
    .normal = glm::vec3(0, -1, 0),
    .vertices =
        {
            glm::vec3(-H, -H, -H),
            glm::vec3(H, -H, -H),
            glm::vec3(H, -H, H),
            glm::vec3(-H, -H, -H),
            glm::vec3(H, -H, H),
            glm::vec3(-H, -H, H),
        },
};
