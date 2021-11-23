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

class Skybox {
 public:
  void loadSkybox(const std::string& a_sDirectory,
                  const std::string& a_sFront,
                  const std::string& a_sBack,
                  const std::string& a_sLeft,
                  const std::string& a_sRight,
                  const std::string& a_sTop,
                  const std::string& a_sBottom);
  void renderSkybox();

  GLuint skyboxSampler;

 private:
  GLuint textureRef[6];
};

#endif  // SKYBOX_H
