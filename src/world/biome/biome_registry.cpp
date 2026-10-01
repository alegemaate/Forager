#include "./biome_registry.h"

#include <asw/asw.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../../utils/utils.h"

// Load biomes from file
void BiomeRegistry::load(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        asw::util::abort_on_error("Cannot find file " + path + " \n Please check your files and try again");
    }

    // Load biomes from xml
    asw::log::info("Loading Biomes");

    // Create buffer
    nlohmann::json doc = nlohmann::json::parse(file);

    // Parse data
    for (auto const& biome : doc)
    {
        // Read xml variables
        // General
        int         biomeID = biome["id"];
        std::string name    = biome["name"];

        // Spawn chance
        int chance = biome["chance"];

        // Mountains
        int mountain_frequency = biome["mountain"]["frequency"];
        int mountain_height    = biome["mountain"]["height"];
        int mountain_radius    = biome["mountain"]["radius"];
        int mountain_steepness = biome["mountain"]["steepness"];

        // Create biome, set variables and add it to the biome list
        Biome newBiome(name, biomeID);

        // Chance of spawn
        newBiome.setChance(chance);

        // Mountains
        newBiome.setMountainRates(mountain_frequency, mountain_height, mountain_radius, mountain_steepness);

        // Resources
        for (auto const& resource : biome["resources"])
        {
            newBiome.addTileFrequency(resource["id"], resource["chance"]);
        }

        // Calc spawn stuff
        newBiome.finish();

        // Add the biome
        biomes.push_back(newBiome);

        // Draw to screen (debug)
        asw::log::progress(static_cast<float>(biomes.size()) / doc.size(), "{} ID: {}", name, biomeID);
    }
}

Biome BiomeRegistry::getBiome(int biomeID)
{
    return biomes.at(biomeID);
}
