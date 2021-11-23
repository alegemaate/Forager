#include "TileTypeManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#include "../utils/utils.h"

TileTypeManager::TileTypeManager() {
  // ctor
}

TileTypeManager::~TileTypeManager() {
  // dtor
}

// Load tiles
void TileTypeManager::load(std::string newFile) {
  // Load biomes from file
  rapidxml::xml_document<> doc;
  std::ifstream file;

  // Check exist
  if (fileExists(newFile.c_str())) {
    file.open(newFile.c_str());
  } else {
    abort_on_error(std::string("Cannot find file " + newFile +
                               " \n Please check your files and try again")
                       .c_str());
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  rapidxml::xml_node<>* allTiles = doc.first_node();

  // Loading
  std::cout << "   TILES\n-------------\n";

  // Load tiles
  for (rapidxml::xml_node<>* cTile = allTiles->first_node("tile"); cTile;
       cTile = cTile->next_sibling()) {
    // Read xml variables
    // General
    int tileID = atoi(cTile->first_attribute("id")->value());
    std::string name = cTile->first_node("name")->value();
    std::string image1 =
        cTile->first_node("images")->first_node("image")->value();
    std::string image2 =
        cTile->first_node("images")->first_node("image2")->value();
    std::string model = cTile->first_node("model")->value();
    int randomness = atoi(cTile->first_node("random")->value());

    std::string attribute_string = cTile->first_node("attribute")->value();
    int attribute = ATTRIBUTE_GAS;

    std::cout << attribute_string;

    // Get attribute
    if (attribute_string == "ATTRIBUTE_GAS") {
      attribute = ATTRIBUTE_GAS;
    } else if (attribute_string == "ATTRIBUTE_SOLID") {
      attribute = ATTRIBUTE_SOLID;
    } else if (attribute_string == "ATTRIBUTE_LIQUID") {
      attribute = ATTRIBUTE_LIQUID;
    }

    // Draw to screen (debug)
    std::cout << "-> Loading Tile:" << name << "  ID:" << tileID
              << "  MODEL:" << model << "  ATTRIBUTE:" << attribute_string
              << "  RANDOMNESS:" << randomness << "\n";

    // Create tile, set variables and add it to the tile list
    TileType newTileType(name, tileID, model, attribute, randomness);

    // Set images
    newTileType.setImages(image1, image2);

    // Add the tile
    tile_defs.push_back(newTileType);
  }

  std::cout << "\n\n";
}

TileType* TileTypeManager::getTileByType(int tileID) {
  return &tile_defs.at(tileID);
}
