#include "BiomeManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../rapidxml/rapidxml.hpp"
#include "../utils/utils.h"

// Load biomes from file
void BiomeManager::load(std::string newFile) {
  rapidxml::xml_document<> doc;
  std::ifstream file;

  // Check exist
  if (fileExists(newFile.c_str())) {
    file.open(newFile.c_str());
  } else {
    abortOnError("Cannot find file " + newFile +
                 " \n Please check your files and try again");
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  rapidxml::xml_node<>* allBiomes = doc.first_node();

  // Load biomes from xml
  std::cout << "   BIOMES\n-------------\n";

  for (rapidxml::xml_node<>* cBiome = allBiomes->first_node("biome"); cBiome;
       cBiome = cBiome->next_sibling()) {
    // Read xml variables
    // General
    int biomeID = atoi(cBiome->first_attribute("id")->value());
    std::string name = cBiome->first_node("name")->value();

    // Spawn chance
    int chance = atoi(cBiome->first_node("chance")->value());

    // Mountains
    int mountain_frequency =
        atoi(cBiome->first_node("mountain")->first_node("frequency")->value());
    int mountain_height =
        atoi(cBiome->first_node("mountain")->first_node("height")->value());
    int mountain_radius =
        atoi(cBiome->first_node("mountain")->first_node("radius")->value());
    int mountain_steepness =
        atoi(cBiome->first_node("mountain")->first_node("steepness")->value());

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
    for (rapidxml::xml_node<>* cResource =
             cBiome->first_node("resources")->first_node("resource");
         cResource; cResource = cResource->next_sibling()) {
      newBiome.addTileFrequency(
          atoi(cResource->first_attribute("tileID")->value()),
          atoi(cResource->value()));
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
