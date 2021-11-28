#include "TileTypeManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../constants/ids.h"
#include "../rapidxml/rapidxml.hpp"
#include "../utils/utils.h"

std::vector<TileType> TileTypeManager::tile_defs;

// Load tiles
void TileTypeManager::load(const std::string& newFile) {
  // Load biomes from file
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

  rapidxml::xml_node<>* allTiles = doc.first_node();

  // Loading
  std::cout << "   TILES\n-------------\n";

  // Load tiles
  for (rapidxml::xml_node<>* xmlTile = allTiles->first_node("tile"); xmlTile;
       xmlTile = xmlTile->next_sibling()) {
    // General
    int id = atoi(xmlTile->first_attribute("id")->value());
    std::string name = xmlTile->first_node("name")->value();

    // Atlas ids
    rapidxml::xml_node<>* xmlAtlas = xmlTile->first_node("atlas");

    AtlasLookup atlasIds;
    atlasIds[0] = atoi(xmlAtlas->first_attribute("top")->value());
    atlasIds[1] = atoi(xmlAtlas->first_attribute("bottom")->value());
    atlasIds[2] = atoi(xmlAtlas->first_attribute("left")->value());
    atlasIds[3] = atoi(xmlAtlas->first_attribute("right")->value());
    atlasIds[4] = atoi(xmlAtlas->first_attribute("front")->value());
    atlasIds[5] = atoi(xmlAtlas->first_attribute("back")->value());

    // Log
    std::cout << "-> Loading Tile:" << name << "  ID:" << id << std::endl;

    // Create tile, set variables and add it to the tile list
    TileType tile(id, atlasIds);

    // Add the tile
    tile_defs.push_back(tile);
  }

  std::cout << "\n\n";
}

TileType* TileTypeManager::getTileByType(int tileID) {
  return &tile_defs.at(tileID);
}
