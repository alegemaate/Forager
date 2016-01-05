#include "audio_3d.h"

audio_3d::audio_3d( std::string file, float newX, float newY, float newZ){
  // Init
  this -> x = newX;
  this -> y = newY;
  this -> z = newZ;
  loadSound( file);
  a3d_play_3D = false;

  a3d_volume = 255;
  a3d_pan = 127;
  a3d_frequency = 1000;
  a3d_loop = false;
}

audio_3d::~audio_3d(){
  //dtor
}

// Load audio into allegro sample
void audio_3d::loadSound( std::string newFileName){
  if(!(a3d_file = load_sample( newFileName.c_str())))
    abort_on_error( ("Could not load sample " + newFileName + ".").c_str());
}

void audio_3d::play( int volume, int pan, int frequency, bool loop){
  a3d_volume = volume;
  a3d_pan = pan;
  a3d_frequency = frequency;
  a3d_loop = loop;
  a3d_play_3D = false;

  stop_sample( a3d_file);
  play_sample( a3d_file, a3d_volume, a3d_pan, a3d_frequency, a3d_loop);
}

void audio_3d::play3D( float *newListenerX, float *newListenerY, float *newListenerZ, int volume, int pan, int frequency, bool loop){
  listenerX = newListenerX;
  listenerY = newListenerY;
  listenerZ = newListenerZ;

  play( volume, pan, frequency, loop);

  a3d_play_3D = true;
}

void audio_3d::update(){
  if( a3d_play_3D){
    a3d_volume_3d = a3d_volume - 5 * (distanceTo3D( x, y, z, *listenerX, *listenerY, *listenerZ));
    if( a3d_volume_3d > 0)
      adjust_sample( a3d_file, a3d_volume_3d, a3d_pan, a3d_frequency, a3d_loop);
    else
      adjust_sample( a3d_file, 0, a3d_pan, a3d_frequency, a3d_loop);
  }
}
