#include "BiomeColorBuffer.h"
#include "Biome.h"

std::vector<unsigned char> GenerateBiomeColorBuffer(TerrainMap& terrainMap)
{
    int width = terrainMap.GetMapWidth();
    int height = terrainMap.GetMapHeight();

    std::vector<unsigned char> buffer;
    buffer.reserve(width * height * 4);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BiomeType biome = terrainMap.GetBiome(x, y);

            Color color;
            color = biomeColors.at(biome);

            buffer.push_back(color.r);
            buffer.push_back(color.g);
            buffer.push_back(color.b);
            buffer.push_back(255);
        }
    }

    return buffer;
}