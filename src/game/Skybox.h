/*
  Skybox
  Allan Legemaate
  04/01/16
  A neato lil skybox loader
*/

#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>

#include <string>
#include <vector>

class Skybox {
 public:
  void loadSkybox(const std::string& pathFront,
                  const std::string& pathBack,
                  const std::string& pathLeft,
                  const std::string& pathRight,
                  const std::string& pathTop,
                  const std::string& pathBottom);
  void render() const;

 private:
  GLuint cubemapTexture{0};

  GLuint vao{0};
  GLuint vbo{0};

  static GLuint loadCubemap(std::vector<std::string> faces);
};

#endif  // SKYBOX_H
