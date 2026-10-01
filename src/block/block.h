/*
  Tile
  Allan Legemaate
  11/11/15
  Class for the tile data (for images see TextureLoader.h)
*/

#pragma once

#include <glm/glm.hpp>

#include "./block_type.h"

class Block
{
  public:
    Block();
    explicit Block(BlockType* type);

    bool isSolid() const
    {
        return tileImpl->isSolid();
    }

    BlockID getType()
    {
        return tileImpl->getType();
    }

    void setType(BlockType* type)
    {
        tileImpl = type;
    }

    BlockType* getTile()
    {
        return tileImpl;
    }

  private:
    BlockType* tileImpl = nullptr;
};
