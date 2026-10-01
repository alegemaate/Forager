#include "./block_type.h"

#include <utility>

BlockType::BlockType(BlockID type, std::string name, AtlasLookup atlasId, BlockModel model, bool solid, bool opaque,
                     bool liquid, core::u8 light)
    : type(type), name(std::move(name)), atlasIds(atlasId), model(model), solid(solid), opaque(opaque), liquid(liquid),
      light(light)
{
}
