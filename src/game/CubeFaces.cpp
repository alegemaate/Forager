#include "CubeFaces.h"

static constexpr float H  = 0.5f;
static constexpr float H2 = 2 * H;

// Helper: same UVs for all faces
std::array<glm::vec2, 4> faceUVs = {glm::vec2{0.f, 1.f}, glm::vec2{1.f, 1.f}, glm::vec2{1.f, 0.f}, glm::vec2{0.f, 0.f}};

FaceDefinition frontFace = {.normal = glm::vec3(0, 0, 1),
                            .vertices =
                                {
                                    glm::vec3(-H, -H, H),
                                    glm::vec3(H, -H, H),
                                    glm::vec3(H, H, H),
                                    glm::vec3(-H, H, H),
                                },
                            .neighbours = {glm::vec3(-H2, -H2, H2), glm::vec3(0, -H2, H2), glm::vec3(H2, -H2, H2),
                                           glm::vec3(H2, 0, H2), glm::vec3(H2, H2, H2), glm::vec3(0, H2, H2),
                                           glm::vec3(-H2, H2, H2), glm::vec3(-H2, 0, H2)}};

FaceDefinition backFace = {.normal = glm::vec3(0, 0, -1),
                           .vertices =
                               {
                                   glm::vec3(H, -H, -H),
                                   glm::vec3(-H, -H, -H),
                                   glm::vec3(-H, H, -H),
                                   glm::vec3(H, H, -H),
                               },
                           .neighbours = //
                           {
                               glm::vec3(H2, -H2, -H2),
                               glm::vec3(0, -H2, -H2),
                               glm::vec3(-H2, -H2, -H2),
                               glm::vec3(-H2, 0, -H2),
                               glm::vec3(-H2, H2, -H2),
                               glm::vec3(0, H2, -H2),
                               glm::vec3(H2, H2, -H2),
                               glm::vec3(H2, 0, -H2),
                           }};

FaceDefinition leftFace = {
    .normal = glm::vec3(-1, 0, 0),
    .vertices =
        {
            glm::vec3(-H, -H, -H),
            glm::vec3(-H, -H, H),
            glm::vec3(-H, H, H),
            glm::vec3(-H, H, -H),
        },
    .neighbours =
        {
            glm::vec3(-H2, -H2, -H2),
            glm::vec3(-H2, -H2, 0),
            glm::vec3(-H2, -H2, H2),
            glm::vec3(-H2, 0, H2),
            glm::vec3(-H2, H2, H2),
            glm::vec3(-H2, H2, 0),
            glm::vec3(-H2, H2, -H2),
            glm::vec3(-H2, 0, -H2),
        },
};

FaceDefinition rightFace = {
    .normal = glm::vec3(1, 0, 0),
    .vertices =
        {
            glm::vec3(H, -H, H),
            glm::vec3(H, -H, -H),
            glm::vec3(H, H, -H),
            glm::vec3(H, H, H),
        },
    .neighbours =
        {
            glm::vec3(H2, -H2, H2),
            glm::vec3(H2, -H2, 0),
            glm::vec3(H2, -H2, -H2),
            glm::vec3(H2, 0, -H2),
            glm::vec3(H2, H2, -H2),
            glm::vec3(H2, H2, 0),
            glm::vec3(H2, H2, H2),
            glm::vec3(H2, 0, H2),
        },
};

FaceDefinition topFace = {.normal = glm::vec3(0, 1, 0),
                          .vertices =
                              {
                                  glm::vec3(-H, H, H),
                                  glm::vec3(H, H, H),
                                  glm::vec3(H, H, -H),
                                  glm::vec3(-H, H, -H),
                              },
                          .neighbours = {glm::vec3(-H2, H2, H2), glm::vec3(0, H2, H2), glm::vec3(H2, H2, H2),
                                         glm::vec3(H2, H2, 0), glm::vec3(H2, H2, -H2), glm::vec3(0, H2, -H2),
                                         glm::vec3(-H2, H2, -H2), glm::vec3(-H2, H2, 0)}};

FaceDefinition bottomFace = {.normal = glm::vec3(0, -1, 0),
                             .vertices =
                                 {
                                     glm::vec3(-H, -H, -H),
                                     glm::vec3(H, -H, -H),
                                     glm::vec3(H, -H, H),
                                     glm::vec3(-H, -H, H),
                                 },
                             .neighbours = {glm::vec3(-H2, -H2, -H2), glm::vec3(0, -H2, -H2), glm::vec3(H2, -H2, -H2),
                                            glm::vec3(H2, -H2, 0), glm::vec3(H2, -H2, H2), glm::vec3(0, -H2, H2),
                                            glm::vec3(-H2, -H2, H2), glm::vec3(-H2, -H2, 0)}};
