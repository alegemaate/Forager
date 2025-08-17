#include "Audio3d.h"

#include "../utils/utils.h"

Audio3d::Audio3d(const std::string& path, float x, float y, float z)
    : x(x), y(y), z(z) {
  loadSound(path);
}

// Load audio into asw sample
void Audio3d::loadSound(const std::string& newFileName) {
  sample = asw::assets::loadSample(newFileName);
  if (!sample) {
    abortOnError("Could not load sample " + newFileName + ".");
  }
}

void Audio3d::play() {
  asw::sound::play(sample, volume, pan, loop);
}

void Audio3d::update(float listenerX, float listenerY, float listenerZ) {
  const auto distance = distanceTo3D(x, y, z, listenerX, listenerY, listenerZ);
  auto volume3D = std::max(volume - static_cast<int>(5.0f * distance), 0);
  // adjust_sample(sample, volume3D, pan, loop); TODO
}
