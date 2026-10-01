/*
  Tile Type Manager
  Allan Legemaate
  24/11/15
  This loads all the types of tiles into a container for access by tile objects.
*/

#pragma once

#include <array>
#include <string>

#include "./block_type.h"

/// @brief Block types, loaded once at start up. Read only after load, so safe to read from worker threads.
class BlockRegistry
{
  public:
    static void load(const std::string& path);

    static const BlockType& get(BlockID blockID)
    {
        return blocks[static_cast<size_t>(blockID)];
    }

  private:
    static std::array<BlockType, static_cast<size_t>(BlockID::Max)> blocks;
};
