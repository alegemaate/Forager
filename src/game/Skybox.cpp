#include "Skybox.h"

#include <SDL3_image/SDL_image.h>
#include <asw/asw.h>
#include <iostream>

#include "../constants/globals.h"
#include "../utils/gl.h"
#include "../utils/utils.h"

#include "../utils/loaders.h"

// Load the skybox
void Skybox::loadSkybox(const std::string& pathFront,
                        const std::string& pathBack,
                        const std::string& pathLeft,
                        const std::string& pathRight,
                        const std::string& pathTop,
                        const std::string& pathBottom) {
  static const float verts[] = {
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  std::vector<std::string> faces{pathRight,  pathLeft,  pathTop,
                                 pathBottom, pathFront, pathBack};
  cubemapTexture = loaders::loadCubemap(faces);

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glBindVertexArray(0);
}

// Render skybox
void Skybox::render() const {
  glDepthFunc(GL_LEQUAL);

  skyShader->activate();
  skyShader->setInt("skybox", 0);

  const auto screenSize = asw::display::getLogicalSize();

  glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));
  glm::mat4 projection =
      glm::perspective(glm::radians(camera->zoom),
                       (float)screenSize.x / (float)screenSize.y, 0.1f, 100.0f);

  skyShader->setMat4("view", view);
  skyShader->setMat4("projection", projection);
  skyShader->setVec3("ambient", lightColor);

  glBindVertexArray(vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  glDepthFunc(GL_LESS);
}
