#include "Skybox.h"

#include <iostream>

#include "../constants/globals.h"
#include "../utils/utils.h"

#include "../utils/loaders.h"

float Skybox::skyboxVertices[108] = {
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

// Load the skybox
void Skybox::loadSkybox(const std::string& pathFront,
                        const std::string& pathBack,
                        const std::string& pathLeft,
                        const std::string& pathRight,
                        const std::string& pathTop,
                        const std::string& pathBottom) {
  std::vector<std::string> faces{pathRight,  pathLeft,  pathTop,
                                 pathBottom, pathFront, pathBack};

  cubemapTexture = loadCubemap(faces);
  std::cout << "Cubemap" << cubemapTexture << std::endl;

  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &Skybox::skyboxVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  skyShader->activate();
  skyShader->setInt("skybox", 0);
}

// Render skybox
void Skybox::render() const {
  glDepthFunc(GL_LEQUAL);

  skyShader->activate();

  glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));
  glm::mat4 projection =
      glm::perspective(glm::radians(camera->zoom),
                       (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);

  skyShader->setMat4("view", view);
  skyShader->setMat4("projection", projection);
  skyShader->setVec3("ambient", lightColor);

  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  glDepthFunc(GL_LESS);
}

GLuint Skybox::loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  for (unsigned int i = 0; i < faces.size(); i++) {
    BITMAP* data = loaders::loadImage(faces[i]);

    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data->w,
                   data->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->dat);
    } else {
      abortOnError("Cubemap texture failed to load at path: " + faces[i]);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}
