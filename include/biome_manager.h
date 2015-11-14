#ifndef BIOME_MANAGER_H
#define BIOME_MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "tools.h"

#include "biome.h"

class biome_manager{
  public:
    biome_manager();
    virtual ~biome_manager();

    // Load biomes
    void load( std::string newFile);

    // Allows communication
    int getNumberBiomes(){ return biomes.size();};
    biome getBiome( int biomeID);

  protected:
  private:
    std::vector<biome> biomes;
};

#endif // BIOME_MANAGER_H
