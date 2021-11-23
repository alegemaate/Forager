#include "Biome.h"

#include "../utils/utils.h"

Biome::Biome(const std::string& name, int id) : name(name), ID(id) {}

// Set mountain rates
void Biome::setMountainRates(int frequency,
                             int height,
                             int radius,
                             int steepness) {
  mountain_frequency = frequency;
  mountain_height = height;
  mountain_radius = radius;
  mountain_steepness = steepness;
}

// Add spawnable tile
void Biome::addTileFrequency(int tileID, int frequency) {
  tileSpawnRates[tileID] = frequency;
}

// Spawn resources randomly according to parameters
int Biome::spawnResource() {
  // Place some stuff
  bool objectPlaced = false;
  int randomGenerateSpawn = 0;
  while (!objectPlaced && numberSpawnableResources > 0) {
    randomGenerateSpawn = random(0, numberSpawnableResources);
    // Choose an object
    if (random(0, tileSpawnRates[tilesSpawnable[randomGenerateSpawn]]) == 1)
      objectPlaced = true;
  }
  return tilesSpawnable[randomGenerateSpawn];
}

// Calc spawn rates
void Biome::finish() {
  for (int i = 0; i < NUMBER_TILE_IDS; i++) {
    if (tileSpawnRates[i] > 0) {
      tilesSpawnable[numberSpawnableResources] = i;
      numberSpawnableResources++;
    }
  }
}
