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
#include "tools.h"

class audio_3d{
  public:
    audio_3d( std::string file, float newX = 0, float newY = 0, float newZ = 0);
    virtual ~audio_3d();

    void loadSound( std::string newFileName);

    void play( int volume = 255, int pan = 127, int frequency = 1000, bool loop = true);
    void play3D( float *newListenerX, float *newListenerY, float *newListenerZ, int volume = 255, int pan = 127, int frequency = 1000, bool loop = true);

    void update();
  protected:
  private:
    float x, y, z;
    float *listenerX, *listenerY, *listenerZ;
    int a3d_volume, a3d_volume_3d, a3d_pan, a3d_frequency;
    bool a3d_loop;
    std::string fileName;
    SAMPLE *a3d_file;

    bool a3d_play_3D;
};

#endif // AUDIO_3D_H
