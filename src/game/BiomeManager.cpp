#include "BiomeManager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../utils/utils.h"

// Load biomes from file
void BiomeManager::load(std::string path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    abortOnError("Cannot find file " + path +
                 " \n Please check your files and try again");
  }

  // Load biomes from xml
  std::cout << "   BIOMES\n-------------\n";

  // Create buffer
  nlohmann::json doc = nlohmann::json::parse(file);

  // Parse data
  for (auto const& biome : doc) {
    // Read xml variables
    // General
    int biomeID = biome["id"];
    std::string name = biome["name"];

    // Spawn chance
    int chance = biome["chance"];

    // Mountains
    int mountain_frequency = biome["mountain"]["frequency"];
    int mountain_height = biome["mountain"]["height"];
    int mountain_radius = biome["mountain"]["radius"];
    int mountain_steepness = biome["mountain"]["steepness"];

    // Draw to screen (debug)
    std::cout << "-> Loading Biome:" << name << " ID:" << biomeID
              << " CHANCE:" << chance << "\n   "
              << " MtnFreq:" << mountain_frequency
              << " MtnHt:" << mountain_height << " MtnRad:" << mountain_radius
              << " MtnSteep:" << mountain_steepness << "\n";

    // Create biome, set variables and add it to the biome list
    Biome newBiome(name, biomeID);

    // Chance of spawn
    newBiome.setChance(chance);

    // Mountains
    newBiome.setMountainRates(mountain_frequency, mountain_height,
                              mountain_radius, mountain_steepness);

    // Resources
    for (auto const& resource : biome["resources"]) {
      newBiome.addTileFrequency(resource["id"], resource["chance"]);
    }

    // Calc spawn stuff
    newBiome.finish();

    // Add the biome
    biomes.push_back(newBiome);
  }

  // Done
  std::cout << "\n";
}

Biome BiomeManager::getBiome(int biomeID) {
  return biomes.at(biomeID);
}
