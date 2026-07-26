#pragma once

#include "TerrainMap.h"

#include <string>

class ImageExporter {
    public:
        void ExportBiomeMapToPPM(TerrainMap &terrainMap, std::string fileName);
    private:

};