/*
  Skybox
  Allan Legemaate
  04/01/16
  A neato lil skybox loader
*/

#ifndef SKYBOX_H
#define SKYBOX_H

#include <allegro.h>

#include <alleggl.h>
#include <string>

class skybox {
 public:
  skybox();
  virtual ~skybox();

  void loadSkybox(std::string a_sDirectory,
                  std::string a_sFront,
                  std::string a_sBack,
                  std::string a_sLeft,
                  std::string a_sRight,
                  std::string a_sTop,
                  std::string a_sBottom);
  void renderSkybox();

  int skyboxSampler;

 protected:
 private:
  BITMAP* texture[6];
  int textureRef[6];

  GLuint vbo;
  GLuint vao;
};

#endif  // SKYBOX_H
