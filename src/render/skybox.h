/*
  Skybox
  Allan Legemaate
  04/01/16
  A neato lil skybox loader
*/

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../render/gpu_program.h"

class Camera;

class Skybox
{
  public:
    void loadSkybox(const std::string& pathFront, const std::string& pathBack, const std::string& pathLeft,
                    const std::string& pathRight, const std::string& pathTop, const std::string& pathBottom);
    void render(const Camera& camera, const glm::vec3& tint) const;

    Skybox() = default;
    ~Skybox();

    // Owns GL handles
    Skybox(const Skybox&)            = delete;
    Skybox& operator=(const Skybox&) = delete;

  private:
    GLuint cubemapTexture{0};

    GLuint vao{0};
    GLuint vbo{0};

    GpuProgram skyShader;

    static GLuint loadCubemap(std::vector<std::string> faces);
};
