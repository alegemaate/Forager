#include "./block_registry.h"

#include <asw/asw.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "../core/Types.h"
#include "../utils/utils.h"

using namespace core;
using namespace std;

// Load tiles
void BlockRegistry::load(const string& path)
{
    ifstream file(path);
    if (!file.is_open())
    {
        asw::util::abort_on_error("Cannot find file '" + path + "'.\nPlease check your files and try again");
    }

    // Loading
    asw::log::info("Loading Tiles");

    // Create buffer
    const nlohmann::json doc = nlohmann::json::parse(file);

    // Parse data
    size_t loaded = 0;
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
        if (id >= blocks.size())
        {
            asw::util::abort_on_error("Invalid tile ID: " + to_string(id));
            continue;
        }

        // Add the tile
        blocks[id] = BlockType(BlockType::fromInt(id), atlasIds);

        // Log current progress
        loaded++;
        asw::log::progress(static_cast<float>(loaded) / doc.size(), "{} ID: {}", name, id);
    }
}

BlockType* BlockRegistry::getTileByType(BlockID blockID)
{
    auto blockIdx = static_cast<u32>(blockID);
    if (blockIdx >= blocks.size())
    {
        throw out_of_range("Block type not found: " + to_string(static_cast<int>(blockID)));
    }

    return &blocks.at(blockIdx);
}
