/*
  Chunk
  Allan Legemaate
  08/01/16
  Block of blocks!
*/

#include "Chunk.h"

#include "../core/Types.h"
#include "./TileTypeManager.h"
#include "./World.h"

using namespace core;

// Construct
Chunk::Chunk(u32 x, u32 z) : index_x(x), index_z(z) {}

void Chunk::generate(World& world, u32 seed)
{
    auto& tileManager = world.getTileManager();

    // STEP 1:
    // 2D Heightmap generation
    const SimplexNoise heightMap = SimplexNoise(0.002f, 0.002f, 2.0f, 0.47f);

    const i32 HEIGHT_OCTAVES = 7;
    const i32 X_OFFSET       = seed + (index_x * CHUNK_WIDTH);
    const i32 Z_OFFSET       = seed + (index_z * CHUNK_LENGTH);
    const i32 HALF_HEIGHT    = CHUNK_HEIGHT / 2;

    for (u32 x = 0; x < CHUNK_WIDTH; x++)
    {
        auto noiseX = static_cast<float>(x + X_OFFSET);

        for (u32 z = 0; z < CHUNK_LENGTH; z++)
        {
            const auto  noiseZ = static_cast<float>(z + Z_OFFSET);
            const float val    = heightMap.fractal(HEIGHT_OCTAVES, noiseX, noiseZ);

            // Height of terrain at this (x,z) position
            // Cache for future steps
            const u32 height = static_cast<u32>((val + 1) * HALF_HEIGHT);
            height_map[x][z] = height;

            for (u32 y = 0; y < CHUNK_HEIGHT; y++)
            {
                // Air
                if (y > height)
                {
                    blk[x][y][z].setType(tileManager.getTileByType(TileID::Air));
                }

                else if (y + 1 > height)
                { // Grass
                    blk[x][y][z].setType(tileManager.getTileByType(TileID::Grass));
                }
                else if (y + 4 > height)
                { // Dirt
                    blk[x][y][z].setType(tileManager.getTileByType(TileID::Dirt));
                }
                else
                { // Stone
                    blk[x][y][z].setType(tileManager.getTileByType(TileID::Stone));
                }
            }
        }
    }

    // STEP 2:
    // Caves
    const SimplexNoise caveMap      = SimplexNoise(0.03f, 0.03f, 2.0f, 0.47f);
    const int          CAVE_OCTAVES = 6; // was 10

    for (u32 x = 0; x < CHUNK_WIDTH; x++)
    {
        auto noiseX = static_cast<float>(x + X_OFFSET);

        for (u32 z = 0; z < CHUNK_LENGTH; z++)
        {
            auto noiseZ = static_cast<float>(z + Z_OFFSET);
            auto height = height_map[x][z];

            for (u32 y = 4; y < height - 4; y++)
            {
                auto noiseY = static_cast<float>(y);
                auto val    = caveMap.fractal(CAVE_OCTAVES, noiseX, noiseZ, noiseY);

                if (val > 0.0f)
                {
                    blk[x][y][z].setType(tileManager.getTileByType(TileID::Air));
                }
            }
        }
    }

    changed = true;
}

Voxel& Chunk::get(u32 x, u32 y, u32 z)
{
    return blk[x][y][z];
}

void Chunk::update(World& world)
{
    if (changed)
    {
        mesh.tessellate(world, glm::vec3(index_x * CHUNK_WIDTH, 0, index_z * CHUNK_LENGTH), blk);
        changed = false;
    }

    if (asw::input::wasKeyPressed(asw::input::Key::F) || asw::input::wasKeyReleased(asw::input::Key::F))
    {
        changed = true;
    }
}

void Chunk::render(World& world)
{
    mesh.render(world, index_x, 0, index_z);
}
