/*
  Audio 3D
  Allan Legemaate
  05/01/16
  3D Sound object
*/

#pragma once

#include <asw/asw.h>
#include <string>

class Audio3d {
 public:
  explicit Audio3d(const std::string& path,
                   float x = 0,
                   float y = 0,
                   float z = 0);

  void loadSound(const std::string& newFileName);

  void play();

  void update(float listenerX, float listenerY, float listenerZ);

 private:
  float x;
  float y;
  float z;

  int volume = 255;
  int pan = 127;
  int loop = false;

  asw::Sample sample = nullptr;
};
