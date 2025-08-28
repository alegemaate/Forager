#include "./block.h"

Block::Block() : tileImpl(nullptr) {}

Block::Block(BlockType* type) : tileImpl(type) {}
