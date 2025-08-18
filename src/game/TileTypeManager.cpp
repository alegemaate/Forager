#include "TileTypeManager.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "../core/Logger.h"
#include "../utils/utils.h"

std::unordered_map<TileID, TileType> TileTypeManager::tileTypes;

// Load tiles
void TileTypeManager::load(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    abortOnError("Cannot find file " + path +
                 " \n Please check your files and try again");
  }

  // Loading
  Logger::heading("Loading Tiles");

  // Create buffer
  const nlohmann::json doc = nlohmann::json::parse(file);

  // Parse data
  for (auto const& tile : doc) {
    // Name of tile
    const std::string name = tile["name"];
    const int id = tile["id"];

    // Atlas
    AtlasLookup atlasIds{};
    atlasIds.top = tile["atlas"]["top"];
    atlasIds.bottom = tile["atlas"]["bottom"];
    atlasIds.left = tile["atlas"]["left"];
    atlasIds.right = tile["atlas"]["right"];
    atlasIds.front = tile["atlas"]["front"];
    atlasIds.back = tile["atlas"]["back"];

    // Add the tile
    auto tileID = intToTileID(id);
    tileTypes.emplace(tileID, TileType(tileID, atlasIds));

    // Log
    Logger::progress(name + " ID:" + std::to_string(id),
                     static_cast<float>(tileTypes.size()) / doc.size());
  }
}

TileType* TileTypeManager::getTileByType(TileID tileID) {
  if (!tileTypes.contains(tileID)) {
    throw std::runtime_error("Tile type not found: " +
                             std::to_string(static_cast<int>(tileID)));
  }
  // Return the tile type
  return &tileTypes.at(tileID);
}
