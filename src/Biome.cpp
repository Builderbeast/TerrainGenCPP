#include "Biome.h"

BiomeType nearInlandTable[5][5] = {
    // H=0                         H=1                         H=2                           H=3                          H=4
    { BiomeType::SnowyPlains,     BiomeType::SnowyPlains,     BiomeType::SnowyTaiga,        BiomeType::SnowyTaiga,       BiomeType::IceSpikes },      // T=0
    { BiomeType::Tundra,          BiomeType::Plains,          BiomeType::Forest,            BiomeType::Taiga,            BiomeType::SnowyTaiga },     // T=1
    { BiomeType::Plains,          BiomeType::Forest,          BiomeType::BirchForest,       BiomeType::Swamp,            BiomeType::Swamp },          // T=2
    { BiomeType::Savanna,         BiomeType::Plains,          BiomeType::Forest,            BiomeType::MangroveSwamp,    BiomeType::MangroveSwamp },  // T=3
    { BiomeType::Desert,          BiomeType::Desert,          BiomeType::Savanna,           BiomeType::MangroveSwamp,    BiomeType::Jungle },         // T=4
};

BiomeType midInlandTable[5][5] = {
    // H=0                         H=1                         H=2                           H=3                          H=4
    { BiomeType::SnowyPlains,     BiomeType::SnowyPlains,     BiomeType::SnowyTaiga,        BiomeType::Taiga,            BiomeType::IceSpikes },
    { BiomeType::Tundra,          BiomeType::Plains,          BiomeType::Forest,            BiomeType::Taiga,            BiomeType::SnowyTaiga },
    { BiomeType::Plains,          BiomeType::Forest,          BiomeType::BirchForest,       BiomeType::DarkForest,       BiomeType::Swamp },
    { BiomeType::Savanna,         BiomeType::Plains,          BiomeType::Forest,            BiomeType::Jungle,           BiomeType::Jungle },
    { BiomeType::Desert,          BiomeType::Desert,          BiomeType::Savanna,           BiomeType::BambooJungle,     BiomeType::Jungle },
};

BiomeType farInlandTable[5][5] = {
    // H=0                         H=1                         H=2                           H=3                         H=4
    { BiomeType::SnowyPlains,     BiomeType::Tundra,          BiomeType::SnowyTaiga,        BiomeType::Taiga,            BiomeType::Taiga },
    { BiomeType::Tundra,          BiomeType::Plains,          BiomeType::Forest,            BiomeType::DarkForest,       BiomeType::Taiga },
    { BiomeType::Savanna,         BiomeType::Plains,          BiomeType::DarkForest,        BiomeType::DarkForest,       BiomeType::Jungle },
    { BiomeType::Badlands,        BiomeType::Savanna,         BiomeType::Forest,            BiomeType::Jungle,           BiomeType::BambooJungle },
    { BiomeType::Badlands,        BiomeType::WoodedBadlands,  BiomeType::Badlands,          BiomeType::Jungle,           BiomeType::Jungle },
};