#include "Skybox.h"

#include <asw/asw.h>

#include "../utils/gl.h"
#include "../utils/loaders.h"
#include "../utils/utils.h"
#include "./World.h"

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

  const std::vector<std::string> faces{pathRight,  pathLeft,  pathTop,
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

  // Load shader
  skyShader.initProgramFromFiles({"sky.vert", "sky.frag"});
}

// Render skybox
void Skybox::render(World& world) const {
  auto& camera = world.getCamera();
  auto& lightColor = world.getLightColor();

  // Shader activation
  glDepthFunc(GL_LEQUAL);
  skyShader.activate();
  skyShader.setInt("skybox", 0);
  skyShader.setMat4("view", glm::mat4(glm::mat3(camera.getViewMatrix())));
  skyShader.setMat4("projection", camera.getProjectionMatrix());
  skyShader.setVec3("ambient", lightColor);

  // Render the skybox
  glBindVertexArray(vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  // Deactivate shader
  skyShader.deactivate();
  glDepthFunc(GL_LESS);
}
