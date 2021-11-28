#include "TileTypeManager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../utils/utils.h"

std::vector<TileType> TileTypeManager::tile_defs;

// Load tiles
void TileTypeManager::load(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    abortOnError("Cannot find file " + path +
                 " \n Please check your files and try again");
  }

  // Loading
  std::cout << "   TILES\n-------------\n";

  // Create buffer
  nlohmann::json doc = nlohmann::json::parse(file);

  // Parse data
  for (auto const& tile : doc) {
    // Name of tile
    std::string name = tile["name"];
    int id = tile["id"];

    // Atlas
    AtlasLookup atlasIds;
    atlasIds.top = tile["atlas"]["top"];
    atlasIds.bottom = tile["atlas"]["bottom"];
    atlasIds.left = tile["atlas"]["left"];
    atlasIds.right = tile["atlas"]["right"];
    atlasIds.front = tile["atlas"]["front"];
    atlasIds.back = tile["atlas"]["back"];

    // Log
    std::cout << "-> Loading Tile:" << name << "  ID:" << id << std::endl;

    // Add the tile
    tile_defs.emplace_back(id, atlasIds);
  }

  std::cout << "\n\n";
}

TileType* TileTypeManager::getTileByType(int tileID) {
  return &tile_defs.at(tileID);
}
