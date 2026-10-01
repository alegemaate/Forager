/*
  Tile Type
  Allan Legemaate
  24/11/15
  Definitions of the tiles in game
*/

#pragma once

#include <stdexcept>
#include <string>

#include "../core/Types.h"

// Tiles. Stored as one byte per block in chunks.
enum class BlockID : core::u8
{
    Air       = 0,
    Grass     = 1,
    Sand      = 2,
    Snow      = 3,
    Stone     = 4,
    Tree      = 5,
    Rock      = 6,
    Water     = 7,
    Ice       = 8,
    Cactus    = 9,
    Lava      = 10,
    Tallgrass = 11,
    GrassSnow = 12,
    TreePine  = 13,
    Temp      = 14, // Temporary tile for testing
    Wood      = 15,
    Dirt      = 16,
    Leaves    = 17,
    Torch     = 18,

    // Terminator
    Max = 19,
};

// How a block is drawn
enum class BlockModel : core::u8
{
    None,  // Not drawn
    Cube,  // Full block
    Torch, // Thin stick
};

struct AtlasLookup
{
    unsigned int top;
    unsigned int bottom;
    unsigned int left;
    unsigned int right;
    unsigned int front;
    unsigned int back;
};

/// @brief Represents an abstracted type of block in the game
class BlockType
{
  public:
    BlockType() = default;

    BlockType(BlockID type, std::string name, AtlasLookup atlasId, BlockModel model, bool solid, bool opaque,
              bool liquid, core::u8 light);

    /// @brief Get underlying tile ID
    BlockID getType() const
    {
        return type;
    }

    const std::string& getName() const
    {
        return name;
    }

    /// @brief Check if tile stops the player
    bool isSolid() const
    {
        return solid;
    }

    /// @brief Check if tile hides the faces behind it and blocks light
    bool isOpaque() const
    {
        return opaque;
    }

    bool isLiquid() const
    {
        return liquid;
    }

    /// @brief Light the block gives off, 0 to 15
    core::u8 getLight() const
    {
        return light;
    }

    BlockModel getModel() const
    {
        return model;
    }

    // Get atlas ids
    const AtlasLookup& getAtlasIds() const
    {
        return atlasIds;
    }

    /// @brief Get BlockID from int representation
    ///
    /// @param id Integer representation of BlockID
    /// @return BlockID
    static BlockID fromInt(core::u32 id)
    {
        if (id >= static_cast<core::u32>(BlockID::Max))
        {
            throw std::runtime_error("Invalid BlockID: " + std::to_string(id));
        }
        return static_cast<BlockID>(id);
    }

  private:
    BlockID     type{BlockID::Air};
    std::string name{"Air"};
    AtlasLookup atlasIds{};
    BlockModel  model{BlockModel::None};
    bool        solid{false};
    bool        opaque{false};
    bool        liquid{false};
    core::u8    light{0};
};
