#include "biome_manager.h"

biome_manager::biome_manager(){

}

biome_manager::~biome_manager()
{
  //dtor
}


// Load biomes from xml
void biome_manager::load( std::string newFile){
  // Load biomes from file
  rapidxml::xml_document<> doc;
  std::ifstream file;

  // Check exist
  if( fexists(newFile.c_str())){
    file.open(newFile.c_str());
  }
  else{
    abort_on_error( std::string("Cannot find file " + newFile + " \n Please check your files and try again").c_str());
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  rapidxml::xml_node<> *allLevels = doc.first_node();

  // Load levels
  for(rapidxml::xml_node<> *cBiome=allLevels-> first_node("biome"); cBiome; cBiome=cBiome->next_sibling()){
    // Read xml variables
    // General
    int biomeID = atoi(cBiome-> first_attribute("id") -> value());
    string name = cBiome-> first_node("name") -> value();

    // Mountains
    int mountain_frequency = atoi(cBiome-> first_node("mountain") -> first_node("frequency") -> value());
    int mountain_height = atoi(cBiome-> first_node("mountain") -> first_node("height") -> value());
    int mountain_radius = atoi(cBiome-> first_node("mountain") -> first_node("radius") -> value());
    int mountain_steepness = atoi(cBiome-> first_node("mountain") -> first_node("steepness") -> value());

    // Draw to screen (debug)
    textprintf_centre_ex(screen,font,640,700,makecol(0,0,0),makecol(255,255,255),"Loading Biome:%s ID:%i",name.c_str(), biomeID);
    textprintf_centre_ex(screen,font,640,760,makecol(0,0,0),makecol(255,255,255),"%i %i %i %i", mountain_frequency, mountain_height, mountain_radius, mountain_steepness);

    // Create biome, set variables and add it to the biome list
    biome newBiome( name, biomeID);

    // Mountains
    newBiome.setMountainRates( mountain_frequency, mountain_height, mountain_radius, mountain_steepness);

    // Resources
    for(rapidxml::xml_node<> *cResource = cBiome-> first_node("resources")-> first_node("resource"); cResource; cResource=cResource->next_sibling()){
      newBiome.addTileFrequency( atoi(cResource -> first_attribute("tileID") -> value()), atoi(cResource -> value()));
      //textprintf_centre_ex(screen,font,640,760,makecol(0,0,0),makecol(255,255,255),"%s %i %i", name.c_str(), atoi(cResource -> first_attribute("id") -> value()), atoi(cResource -> value()));
    }

    // Calc spawn stuff
    newBiome.finish();

    // Add the biome
    biomes.push_back( newBiome);

  }
}

biome biome_manager::getBiome( int biomeID){
  return biomes.at( biomeID);
}
