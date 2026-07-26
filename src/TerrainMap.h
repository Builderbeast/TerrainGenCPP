#pragma once

#include "NoiseGenerator.h"
#include "Biome.h"
#include <vector>

class TerrainMap {
public:
    TerrainMap(int width, int height, unsigned int seed);

    void Generate();
    double GetHeight(int x, int y);
    double GetHumidity(int x, int y);
    double GetTemperature(int x, int y);
    double GetErosion(int x, int y);
    double GetPV(double weirdness);
    BiomeType GetBiome(int x, int y);

    int GetMapWidth();
    int GetMapHeight();
private:
    int width, height;
    std::vector<std::vector<double>> continentalnessMap;
    std::vector<std::vector<double>> humidityMap;
    std::vector<std::vector<double>> temperatureMap;
    std::vector<std::vector<double>> oceanTemperatureMap;
    std::vector<std::vector<double>> erosionMap;
    std::vector<std::vector<double>> weirdnessMap;
    std::vector<std::vector<double>> mountainWeirdnessMap;
    std::vector<std::vector<BiomeType>> biomeMap;

    NoiseGenerator continentalnessNoise;
    NoiseGenerator humAndTempNoise;
    NoiseGenerator erosionNoise;
    NoiseGenerator weirdnessNoise;

    double mountainousErosionThreshold = 1.0;


    BiomeType ClassifyOcean(bool isDeep, int temperatureLevel);
    BiomeType ClassifyMountain(int erosionLevel, double mountainWeirdness, int temperatureLevel);
    BiomeType ClassifyBiome(double humidity, double temperature, double oceanTemperature, double continentalness, double erosion, double weirdness, double mountainWeirdness);
    BiomeType ClassifyLandBiome(ContinentalnessLevel level, int temperatureLevel, int humidityLevel);
    ContinentalnessLevel GetContinentalnessLevel(double continentalness);
    int GetTemperatureLevel(double temperature);
    int GetHumidityLevel(double humidity);
    int GetErosionLevel(double erosion);
    PVLevel GetPVLevel(double pv);
};