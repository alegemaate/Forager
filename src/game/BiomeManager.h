/*
  Biome Manager
  Allan Legemaate
  11/11/15
  This loads the biomes from xml and loads them into its biomes vector for
  reference
*/

#ifndef BIOME_MANAGER_H
#define BIOME_MANAGER_H

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

 protected:
 private:
  std::vector<Biome> biomes;
};

#endif  // BIOME_MANAGER_H
