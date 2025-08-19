/*
  Biome Manager
  Allan Legemaate
  11/11/15
  This loads the biomes from xml and loads them into its biomes vector for
  reference
*/

#pragma once

#include <string>
#include <vector>

#include "Biome.h"

class BiomeManager {
 public:
  // Load biomes
  void load(std::string newFile);

  // Allows communication
  int getNumberBiomes() { return biomes.size(); };
  Biome getBiome(int biomeID);

 private:
  std::vector<Biome> biomes;
};
