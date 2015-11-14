#include "biome.h"

biome::biome(std::string newName, int newID){
  name = newName;
  ID = newID;

  // Set spawn rates to 0
  for( int i = 0; i < 100; i ++)
    tileSpawnRates[i] = 0;

  // Set mountain rates to 0
  int mountain_frequency = 0;
  int mountain_height = 0;
  int mountain_radius = 0;
  int mountian_steepness = 0;
}

biome::~biome(){
  //dtor
}

// Set mountain rates
void biome::setMountainRates( int newFrequency, int newHeight, int newRadius, int newSteepness){
  mountain_frequency = newFrequency;
  mountain_height = newHeight;
  mountain_radius = newRadius;
  mountian_steepness = newSteepness;
}

// Add spawnable tile
void biome::addTileFrequency( int tileID, int frequency){
  //tileSpawnRates[tileID] = frequency;
}
