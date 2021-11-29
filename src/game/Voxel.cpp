#include "Voxel.h"

// Constructor
Voxel::Voxel(glm::u8vec3 position, TileType* type) : position(position) {
  setType(type);
}

// Set tile type
void Voxel::setType(TileType* val) {
  tileImpl = val;
}
