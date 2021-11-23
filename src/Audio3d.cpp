#include "Audio3d.h"

#include "utils/utils.h"

Audio3d::Audio3d(const std::string& path, float x, float y, float z)
    : x(x), y(y), z(z) {
  loadSound(path);
}

// Load audio into allegro sample
void Audio3d::loadSound(const std::string& newFileName) {
  a3d_file = load_sample(newFileName.c_str());

  if (!a3d_file) {
    abort_on_error(("Could not load sample " + newFileName + ".").c_str());
  }
}

void Audio3d::play() {
  stop_sample(a3d_file);
  play_sample(a3d_file, volume, pan, frequency, loop);
}

void Audio3d::update(float listenerX, float listenerY, float listenerZ) {
  const auto distance = distanceTo3D(x, y, z, listenerX, listenerY, listenerZ);

  auto volume3D = volume - 5 * static_cast<int>(distance);

  if (volume3D < 0) {
    volume3D = 0;
  }

  adjust_sample(a3d_file, volume3D, pan, frequency, loop);
}
