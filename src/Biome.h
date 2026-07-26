#pragma once
#include <map>

enum class BiomeType {
    DeepOcean, Ocean, ColdOcean, LukewarmOcean, WarmOcean, DeepColdOcean, DeepLukewarmOcean, Beach, Desert, Savanna, Plains, Forest, BirchForest, DarkForest, Jungle, BambooJungle, Swamp, MangroveSwamp, Taiga, SnowyTaiga, Tundra, SnowyPlains, IceSpikes,
    Badlands, WoodedBadlands, River, FrozenRiver, WindsweptHills, Mountain, JaggedPeaks, FrozenPeaks, StonyPeaks,
    COUNT
};

struct BiomePoint {
    double idealHumidity, idealTemperature;
};

struct BiomeHeightProfile {
    double baseElevation, amplitude;
    int octaves;
    double lacunarity, persistence;
};

struct Color { unsigned char r, g, b; };

enum class ContinentalnessLevel {
    DeepOcean, Ocean, Coast, NearInland, MidInland, FarInland
};

extern BiomeType nearInlandTable[5][5];
extern BiomeType midInlandTable[5][5];
extern BiomeType farInlandTable[5][5];

enum class PVLevel { Valleys, Low, Mid, High, Peaks };


const std::map<BiomeType, Color> biomeColors = {
    // Water
    { BiomeType::DeepOcean,         { 15, 40, 120 } },
    { BiomeType::Ocean,             { 30, 80, 180 } },
    { BiomeType::ColdOcean,         { 35, 90, 175 } },
    { BiomeType::LukewarmOcean,     { 15, 115, 185 } },
    { BiomeType::WarmOcean,         { 15, 145, 175 } },
    { BiomeType::DeepColdOcean,     { 18, 50, 105 } },
    { BiomeType::DeepLukewarmOcean, { 2, 90, 128 } },
    { BiomeType::Beach,             { 230, 220, 150 } },

    { BiomeType::River,             { 40, 100, 190 } },
    { BiomeType::FrozenRiver,       { 170, 210, 230 } },


    // Structural
    { BiomeType::WindsweptHills,    { 120, 115, 100 } },
    { BiomeType::JaggedPeaks,       { 220, 220, 225 } },
    { BiomeType::Mountain,          { 150, 150, 150 } },
    { BiomeType::FrozenPeaks,       { 235, 240, 245 } },
    { BiomeType::StonyPeaks,        { 190, 185, 175 } },

    // Hot & dry
    { BiomeType::Desert,            { 220, 190, 100 } },
    { BiomeType::Savanna,           { 189, 183, 90 } },

    // Temperate / Moderate
    { BiomeType::Plains,            { 145, 200, 80 } },
    { BiomeType::Forest,            { 47, 140, 60 } },
    { BiomeType::BirchForest,       { 130, 175, 90 } },
    { BiomeType::DarkForest,        { 30, 90, 40 } },

    // Hot & wet
    { BiomeType::Jungle,            { 20, 130, 40 } },
    { BiomeType::BambooJungle,      { 90, 170, 60 } },
    { BiomeType::Swamp,             { 80, 100, 60 } },
    { BiomeType::MangroveSwamp,     { 60, 110, 80 } },

    // Cold
    { BiomeType::Taiga,             { 55, 110, 90 } },
    { BiomeType::SnowyTaiga,        { 150, 190, 180 } },
    { BiomeType::Tundra,            { 170, 190, 170 } },
    { BiomeType::SnowyPlains,       { 235, 235, 240 } },
    { BiomeType::IceSpikes,         { 200, 230, 235 } },

    // Badlands
    { BiomeType::Badlands,          { 190, 110, 60 } },
    { BiomeType::WoodedBadlands,    { 140, 110, 65 } },
};