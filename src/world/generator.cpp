#include "./generator.h"

#include <algorithm>

#include "../core/SimplexNoise.h"

std::unique_ptr<ChunkData> generator::generateChunk(ChunkKey key, u32 seed)
{
    auto data = std::make_unique<ChunkData>();

    // STEP 1:
    // 2D Heightmap generation
    const SimplexNoise heightMap(0.002f, 0.002f, 2.0f, 0.47f);
    constexpr i32      HEIGHT_OCTAVES = 7;
    constexpr i32      HALF_HEIGHT    = CHUNK_HEIGHT / 2;

    // The seed moves the sample window, x and z by different amounts so worlds are not mirrored
    const f64 offsetX = static_cast<f64>(seed) + (static_cast<f64>(key.x) * CHUNK_WIDTH);
    const f64 offsetZ = (static_cast<f64>(seed) * 0.5) + 10007.0 + (static_cast<f64>(key.z) * CHUNK_LENGTH);

    u32 heights[CHUNK_WIDTH][CHUNK_LENGTH]{};
    u32 maxY = 0;

    for (u32 x = 0; x < CHUNK_WIDTH; x++)
    {
        const auto noiseX = static_cast<f32>(offsetX + x);

        for (u32 z = 0; z < CHUNK_LENGTH; z++)
        {
            const auto  noiseZ = static_cast<f32>(offsetZ + z);
            const float val    = heightMap.fractal(HEIGHT_OCTAVES, noiseX, noiseZ);

            // Height of terrain at this (x,z) position, cached for the cave step
            const auto height = static_cast<u32>(
                std::clamp(static_cast<i32>((val + 1) * HALF_HEIGHT), 1, static_cast<i32>(CHUNK_HEIGHT) - 2));
            heights[x][z] = height;

            // Low land by the water is beach
            const bool beach = static_cast<i32>(height) <= SEA_LEVEL + 1;

            for (u32 y = 0; y <= height; y++)
            {
                BlockID id = BlockID::Stone;

                if (y == height)
                {
                    id = beach ? BlockID::Sand : BlockID::Grass;
                }
                else if (y + 4 > height)
                {
                    id = beach ? BlockID::Sand : BlockID::Dirt;
                }

                data->set(x, y, z, id);
            }

            // Fill the sea
            for (auto y = static_cast<i32>(height) + 1; y <= SEA_LEVEL; y++)
            {
                data->set(x, static_cast<u32>(y), z, BlockID::Water);
            }

            maxY = std::max({maxY, height, static_cast<u32>(SEA_LEVEL)});
        }
    }

    // STEP 2:
    // Caves
    const SimplexNoise caveMap(0.03f, 0.03f, 2.0f, 0.47f);
    constexpr i32      CAVE_OCTAVES = 6;
    const auto         caveY        = static_cast<f32>(seed % 1000);

    for (u32 x = 0; x < CHUNK_WIDTH; x++)
    {
        const auto noiseX = static_cast<f32>(offsetX + x);

        for (u32 z = 0; z < CHUNK_LENGTH; z++)
        {
            const auto noiseZ = static_cast<f32>(offsetZ + z);
            const auto height = heights[x][z];

            // y + 4 < height, not y < height - 4, which wraps when height < 4.
            // Keeps a roof, so the sea does not pour in.
            for (u32 y = 4; y + 4 < height; y++)
            {
                const auto val = caveMap.fractal(CAVE_OCTAVES, noiseX, noiseZ, static_cast<f32>(y) + caveY);

                if (val > 0.0f)
                {
                    data->set(x, y, z, BlockID::Air);
                }
            }
        }
    }

    data->maxY = maxY;
    return data;
}
