/*
  Tile Type
  Allan Legemaate
  24/11/15
  Definitions of the tiles in game
*/

#pragma once

#include <array>
#include <stdexcept>
#include <string>

#include "../core/Types.h"

// Tiles
enum class TileID
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
    Johnny    = 15, // Special tile for Johnny
    Dirt      = 16,
    Leaves    = 17,

    // Terminator
    Max = 18,
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

/// @brief Represents an abstracted type of tile in the game
class TileType
{
  public:
    TileType() = default;

    TileType(TileID type, AtlasLookup atlasId);

    /// @brief Get underlying tile ID
    ///
    /// @return TileID
    TileID getType() const
    {
        return type;
    }

    // Get atlas ids
    const AtlasLookup& getAtlasIds() const
    {
        return atlasIds;
    }

    /// @brief Get TileID from int representation
    ///
    /// @param id Integer representation of TileID
    /// @return TileID
    static TileID fromInt(core::u32 id)
    {
        if (id < static_cast<core::u32>(TileID::Air) || id > static_cast<core::u32>(TileID::Max))
        {
            throw std::runtime_error("Invalid TileID: " + std::to_string(id));
        }
        return static_cast<TileID>(id);
    }

  private:
    TileID      type;
    AtlasLookup atlasIds;
};
