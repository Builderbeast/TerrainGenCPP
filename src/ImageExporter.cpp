#include "ImageExporter.h"
#include "TerrainMap.h"
#include "Biome.h"

#include <string>
#include <fstream>

void ImageExporter::ExportBiomeMapToPPM(TerrainMap &terrainMap, std::string fileName)
{
    std::ofstream file(fileName, std::ios::binary);

    int width = terrainMap.GetMapWidth();
    int height = terrainMap.GetMapHeight();

    file << "P6\n" << width << " " << height << "\n255\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BiomeType biome = terrainMap.GetBiome(x, y);
            Color color = biomeColors.at(biome);

            file.put(color.r);
            file.put(color.g);
            file.put(color.b);
        }
    }

    file.close();
}