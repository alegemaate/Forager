#include "Voxel.h"

Voxel::Voxel() : tileImpl(nullptr) {}

Voxel::Voxel(TileType* type) : tileImpl(type) {}
