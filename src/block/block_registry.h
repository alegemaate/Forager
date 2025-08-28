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

class BlockRegistry
{
  public:
    void       load(const std::string& path);
    BlockType* getTileByType(BlockID blockID);

  private:
    std::array<BlockType, static_cast<size_t>(BlockID::Max)> blocks;
};
