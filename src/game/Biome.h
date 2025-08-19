/*
  Biome
  Allan Legemaate
  14/11/15
  Class for the biome data for quick access by tiles
*/

#pragma once

#include <string>

#define NUMBER_TILE_IDS 100

class Biome {
 public:
  Biome(const std::string& name, int id);

  // Get mountain info
  int getMountainFrequency() const { return mountain_frequency; }
  int getMountainHeight() const { return mountain_height; }
  int getMountainRadius() const { return mountain_radius; }
  int getMountainSteepness() const { return mountain_steepness; }

  // Get spawn rate
  int getSpawnRate(int tileID) { return tileSpawnRates[tileID]; }

  // Get name
  std::string getName() { return name; }

  // Get ID
  int getID() const { return ID; }

  // Get/Set spawn chance
  void setChance(int newChance) { spawnChance = newChance; }
  int getChance() const { return spawnChance; }

  // Mountain info
  void setMountainRates(int frequency, int height, int radius, int steepness);

  // Adds tile (by id) to the creation list
  void addTileFrequency(int tileID, int frequency);

  // Get random resource spawn
  int spawnResource();

  // Finishing biome calculations
  void finish();

 protected:
 private:
  std::string name;
  int ID;

  int tileSpawnRates[NUMBER_TILE_IDS]{};
  int tilesSpawnable[NUMBER_TILE_IDS]{};
  int numberSpawnableResources = 0;

  int mountain_frequency = 0;
  int mountain_height = 0;
  int mountain_radius = 0;
  int mountain_steepness = 0;

  int spawnChance = 0;
};
