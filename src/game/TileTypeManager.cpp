#include "TileTypeManager.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "../core/Logger.h"
#include "../core/Types.h"
#include "../utils/utils.h"

using namespace core;
using namespace std;

// Load tiles
void TileTypeManager::load(const string& path)
{
    ifstream file(path);
    if (!file.is_open())
    {
        abortOnError("Cannot find file '" + path + "'.\nPlease check your files and try again");
    }

    // Loading
    Logger::heading("Loading Tiles");

    // Create buffer
    const nlohmann::json doc = nlohmann::json::parse(file);

    // Parse data
    for (auto const& tile : doc)
    {
        // Name of tile
        const string name = tile["name"];
        const u32    id   = tile["id"];

        // Atlas
        const AtlasLookup atlasIds{
            .top    = tile["atlas"]["top"],
            .bottom = tile["atlas"]["bottom"],
            .left   = tile["atlas"]["left"],
            .right  = tile["atlas"]["right"],
            .front  = tile["atlas"]["front"],
            .back   = tile["atlas"]["back"],
        };

        // Check index validity
        if (id >= tileTypes.size())
        {
            abortOnError("Invalid tile ID: " + to_string(id));
            continue;
        }

        // Add the tile
        tileTypes[id] = TileType(TileType::fromInt(id), atlasIds);

        // Log current progress
        Logger::progress(name + " ID:" + to_string(id), static_cast<float>(tileTypes.size()) / doc.size());
    }
}

TileType* TileTypeManager::getTileByType(TileID tileID)
{
    auto tileIdx = static_cast<u32>(tileID);
    if (tileIdx >= tileTypes.size())
    {
        throw out_of_range("Tile type not found: " + to_string(static_cast<int>(tileID)));
    }

    return &tileTypes.at(tileIdx);
}
