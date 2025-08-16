#include "Audio3d.h"

#include "../utils/utils.h"

Audio3d::Audio3d(const std::string& path, float x, float y, float z)
    : x(x), y(y), z(z) {
  loadSound(path);
}

// Load audio into asw sample
void Audio3d::loadSound(const std::string& newFileName) {
  a3d_file = asw::assets::loadSample(newFileName);

  if (!a3d_file) {
    abortOnError("Could not load sample " + newFileName + ".");
  }
}

void Audio3d::play() {
  asw::sound::play(a3d_file, volume, pan, loop);
}

void Audio3d::update(float listenerX, float listenerY, float listenerZ) {
  const auto distance = distanceTo3D(x, y, z, listenerX, listenerY, listenerZ);

  auto volume3D = volume - 5 * static_cast<int>(distance);

  if (volume3D < 0) {
    volume3D = 0;
  }

  // adjust_sample(a3d_file, volume3D, pan, loop); TODO
}
