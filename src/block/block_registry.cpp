#include "./block_registry.h"

#include <asw/asw.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "../core/Types.h"
#include "../utils/utils.h"

using namespace core;
using namespace std;

std::array<BlockType, static_cast<size_t>(BlockID::Max)> BlockRegistry::blocks{};

namespace
{
BlockModel parseModel(const string& model)
{
    if (model == "MODEL_NONE")
    {
        return BlockModel::None;
    }
    if (model == "MODEL_TORCH")
    {
        return BlockModel::Torch;
    }

    // Plant models are not drawn yet, so they are cubes
    return BlockModel::Cube;
}
} // namespace

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

        const string attribute = tile["attribute"];
        const bool   liquid    = attribute == "ATTRIBUTE_LIQUID";
        const bool   solid     = attribute == "ATTRIBUTE_SOLID";
        const auto   model     = parseModel(tile["model"]);

        // Only full solid blocks hide what is behind them
        const bool opaque = solid && model == BlockModel::Cube;
        const u8   light  = tile.value("light", 0);

        // Add the tile
        blocks[id] = BlockType(BlockType::fromInt(id), name, atlasIds, model, solid, opaque, liquid, light);

        // Log current progress
        loaded++;
        asw::log::progress(static_cast<float>(loaded) / doc.size(), "{} ID: {}", name, id);
    }
}
