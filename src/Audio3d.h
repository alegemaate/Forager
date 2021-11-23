/*
  Audio 3D
  Allan Legemaate
  05/01/16
  3D Sound object
*/

#ifndef AUDIO_3D_H
#define AUDIO_3D_H

#include <allegro.h>
#include <string>

class Audio3d {
 public:
  explicit Audio3d(const std::string& path,
                   float x = 0,
                   float y = 0,
                   float z = 0);
  virtual ~Audio3d() = default;

  void loadSound(const std::string& newFileName);

  void play();

  void update(float listenerX, float listenerY, float listenerZ);

 private:
  float x;
  float y;
  float z;

  int volume = 255;
  int pan = 127;
  int frequency = 1000;
  bool loop = false;

  SAMPLE* a3d_file = nullptr;
};

#endif  // AUDIO_3D_H
