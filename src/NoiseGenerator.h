#pragma once

class NoiseGenerator {
    public:
        NoiseGenerator(unsigned int seed);
        double PerlinNoise(double x, double y);
        double FractalNoise(double x, double y, int octaves, double lacunarity, double persistence);

    private:
        int p[512];

        void BuildPermutationTable(unsigned int seed);
};