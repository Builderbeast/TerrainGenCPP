#include "TerrainMap.h"
#include "Biome.h"

#include <map>
#include <cmath>
#include <limits>


TerrainMap::TerrainMap(int width, int height, unsigned int seed)
    : humAndTempNoise(seed), continentalnessNoise(seed + 1), erosionNoise(seed + 2), weirdnessNoise(seed + 3)
{
    this->width = width;
    this->height = height;

    continentalnessMap.resize(height, std::vector<double>(width));
    humidityMap.resize(height, std::vector<double>(width));
    temperatureMap.resize(height, std::vector<double>(width));
    oceanTemperatureMap.resize(height, std::vector<double>(width));
    erosionMap.resize(height, std::vector<double>(width));
    weirdnessMap.resize(height, std::vector<double>(width));
    mountainWeirdnessMap.resize(height, std::vector<double>(width));
    biomeMap.resize(height, std::vector<BiomeType>(width));

    Generate();
}

void TerrainMap::Generate()
{
    double continentalnessScale = 0.001;
    double heightFreqScale = 0.0007;
    double humidityFreqScale = 0.005;
    double temperatureFreqScale = 0.005;
    double oceanTemperatureFreqMulti = 0.5;
    double erosionFreqScale = 0.003;
    double weirdnessFreqScale = 0.001;
    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            double continentalnessAtPoint = continentalnessNoise.FractalNoise(x * continentalnessScale, y * continentalnessScale, 4, 1.6, 0.25);
            double humidityAtPoint = humAndTempNoise.FractalNoise(x * humidityFreqScale, y * humidityFreqScale, 4, 1.8, 0.35);
            double temperatureAtPoint = humAndTempNoise.FractalNoise(x * temperatureFreqScale, y * temperatureFreqScale, 4, 1.8, 0.35);
            double oceanTemperatureAtPoint = humAndTempNoise.FractalNoise(x * temperatureFreqScale * oceanTemperatureFreqMulti, y * temperatureFreqScale * oceanTemperatureFreqMulti, 1, 1.8, 0.35);
            double erosionAtPoint = erosionNoise.FractalNoise(x * erosionFreqScale, y * erosionFreqScale, 2, 1.5, 0.4);
            double weirdnessAtPoint = weirdnessNoise.FractalNoise(x * weirdnessFreqScale, y * weirdnessFreqScale, 3, 1.8, 0.35);
            double mountainWeirdnessAtPoint = weirdnessNoise.FractalNoise((x + 5000) * weirdnessFreqScale, (y + 5000) * weirdnessFreqScale, 3, 1.8, 0.35);

            continentalnessAtPoint = (continentalnessAtPoint + 1) / 2;
            humidityAtPoint = (humidityAtPoint + 1) / 2;

            continentalnessMap[y][x] = continentalnessAtPoint;
            humidityMap[y][x] = humidityAtPoint;
            temperatureMap[y][x] = temperatureAtPoint;
            oceanTemperatureMap[y][x] = oceanTemperatureAtPoint;
            erosionMap[y][x] = erosionAtPoint;
            weirdnessMap[y][x] = weirdnessAtPoint;
            mountainWeirdnessMap[y][x] = mountainWeirdnessAtPoint;
            biomeMap[y][x] = ClassifyBiome(humidityAtPoint, temperatureAtPoint, oceanTemperatureAtPoint, continentalnessAtPoint, erosionAtPoint, weirdnessAtPoint, mountainWeirdnessAtPoint);
        }
    }
}

double TerrainMap::GetHumidity(int x, int y)
{
    return humidityMap[y][x];
}

double TerrainMap::GetTemperature(int x, int y)
{
    return temperatureMap[y][x];
}

double TerrainMap::GetErosion(int x, int y)
{
    return erosionMap[y][x];
}

double TerrainMap::GetPV(double weirdness)
{
    return 1.0 - std::abs((3.0 * std::abs(weirdness)) - 2.0);
}

BiomeType TerrainMap::GetBiome(int x, int y)
{
    return biomeMap[y][x];
}

int TerrainMap::GetMapWidth()
{
    return width;
}

int TerrainMap::GetMapHeight()
{
    return height;
}

ContinentalnessLevel TerrainMap::GetContinentalnessLevel(double continentalness)
{
    if (continentalness < 0.20)
        return ContinentalnessLevel::DeepOcean;
    if (continentalness < 0.40)
        return ContinentalnessLevel::Ocean;
    if (continentalness < 0.42)
        return ContinentalnessLevel::Coast;
    if (continentalness < 0.55)
        return ContinentalnessLevel::NearInland;
    if (continentalness < 0.73)
        return ContinentalnessLevel::MidInland;
    return ContinentalnessLevel::FarInland;
}

int TerrainMap::GetTemperatureLevel(double temperature)
{
    if (temperature < -0.45)
        return 0;
    if (temperature < -0.15)
        return 1;
    if (temperature < 0.2)
        return 2;
    if (temperature < 0.55)
        return 3;
    return 4;
}

int TerrainMap::GetHumidityLevel(double humidity)
{
    if (humidity < -0.35)
        return 0;
    if (humidity < -0.1)
        return 1;
    if (humidity < 0.1)
        return 2;
    if (humidity < 0.3)
        return 3;
    return 4;
}

int TerrainMap::GetErosionLevel(double erosion)
{
    if (erosion < -0.78)   return 0;
    if (erosion < -0.375)  return 1;
    if (erosion < -0.2225) return 2;
    if (erosion < 0.05)    return 3;
    if (erosion < 0.45)    return 4;
    if (erosion < 0.55)    return 5;
    return 6;
}

PVLevel TerrainMap::GetPVLevel(double pv)
{
    if (pv < -0.95) return PVLevel::Valleys;
    if (pv < -0.2)  return PVLevel::Low;
    if (pv < 0.2)   return PVLevel::Mid;
    if (pv < 0.7)   return PVLevel::High;
    return PVLevel::Peaks;
}

BiomeType TerrainMap::ClassifyOcean(bool isDeep, int temperatureLevel)
{
    int oceanTier;
    if (temperatureLevel <= 1) oceanTier = 0;
    else if (temperatureLevel == 2) oceanTier = 1;
    else oceanTier = 2;

    if (isDeep)
    {
        if (oceanTier == 0) return BiomeType::DeepColdOcean;
        if (oceanTier == 1) return BiomeType::DeepOcean;
        return BiomeType::DeepLukewarmOcean;
    }
    else
    {
        if (oceanTier == 0) return BiomeType::ColdOcean;
        if (oceanTier == 1) return BiomeType::Ocean;
        return BiomeType::WarmOcean;
    }
}

BiomeType TerrainMap::ClassifyMountain(int erosionLevel, double mountainWeirdness, int temperatureLevel)
{
    bool veryMountainous = erosionLevel <= 1;

    double pv = GetPV(mountainWeirdness);
    PVLevel pvLevel = GetPVLevel(pv);

    if (veryMountainous && pvLevel == PVLevel::Peaks)
    {
        if (temperatureLevel <= 1)
            return BiomeType::FrozenPeaks;
        if (temperatureLevel == 3)
            return BiomeType::StonyPeaks;

        return BiomeType::JaggedPeaks;
    }
    if (veryMountainous && pvLevel == PVLevel::High)
        return BiomeType::Mountain;
    if (pvLevel == PVLevel::Low || pvLevel == PVLevel::Mid)
        return BiomeType::WindsweptHills;

    return BiomeType::Mountain;
}


BiomeType TerrainMap::ClassifyBiome(double humidity, double temperature, double oceanTemperature, double continentalness, double erosion, double weirdness, double mountainWeirdness)
{
    ContinentalnessLevel level = GetContinentalnessLevel(continentalness);
    int temperatureLevel = GetTemperatureLevel(temperature);

    if (level == ContinentalnessLevel::DeepOcean)
        return ClassifyOcean(true, GetTemperatureLevel(oceanTemperature));
    if (level == ContinentalnessLevel::Ocean)
        return ClassifyOcean(false, GetTemperatureLevel(oceanTemperature));

    double pv = GetPV(weirdness);
    PVLevel pvLevel = GetPVLevel(pv);

    int erosionLevel = GetErosionLevel(erosion);
    if (erosionLevel <= mountainousErosionThreshold)
        return ClassifyMountain(erosionLevel, mountainWeirdness, temperatureLevel);
    
    if (pvLevel == PVLevel::Valleys)
        return (temperatureLevel == 0) ? BiomeType::FrozenRiver : BiomeType::River;

    if (level == ContinentalnessLevel::Coast)
        return BiomeType::Beach;

    int humidityLevel = GetHumidityLevel(humidity);

    return ClassifyLandBiome(level, temperatureLevel, humidityLevel);
}

BiomeType TerrainMap::ClassifyLandBiome(ContinentalnessLevel level, int temperatureLevel, int humidityLevel)
{
    if (level == ContinentalnessLevel::NearInland)
        return nearInlandTable[temperatureLevel][humidityLevel];
    if (level == ContinentalnessLevel::MidInland)
        return midInlandTable[temperatureLevel][humidityLevel];

    return farInlandTable[temperatureLevel][humidityLevel];
}