#ifndef BIOME_H
#define BIOME_H

#include <string>

class biome{
  public:
    biome(std::string newName, int newID);
    virtual ~biome();

    // Get mountain info
    int getMountainFrequency(){ return mountain_frequency; }
    int getMountainHeight(){ return mountain_height; }
    int getMountainRadius(){ return mountain_radius; }
    int getMountainSteepness(){ return mountian_steepness; }

    // Get spawn rate
    int getSpawnRate( int tileID){ return tileSpawnRates[tileID]; }

    // Get name
    std::string getName(){ return name; }

    // Get ID
    int getID(){ return ID; }

    // Mountain info
    void setMountainRates( int newFrequency, int newHeight, int newRadius, int newSteepness);

    // Adds tile (by id) to the creation list
    void addTileFrequency( int tileID, int frequency);
  protected:
  private:
    std::string name;
    int ID;

    int tileSpawnRates[100];

    int mountain_frequency;
    int mountain_height;
    int mountain_radius;
    int mountian_steepness;
};

#endif // BIOME_H
