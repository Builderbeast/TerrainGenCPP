#include "NoiseGenerator.h"
#include <cmath>
#include <string>
#include <random>
#include <numeric>
#include <algorithm>

// Private methods
double fade(double t) {
    return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
}

double grad(int hash, double x, double y) {
    switch (hash & 3) {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        case 3: return -x - y;
        case 4: return  x;
        case 5: return -x;
        case 6: return  y;
        case 7: return -y;
    }
}

double lerp(double a, double b, double t) {
    return a + t * (b - a);
}

// NoiseGenerator class
NoiseGenerator::NoiseGenerator(unsigned int seed) {
    BuildPermutationTable(seed);
}

void NoiseGenerator::BuildPermutationTable(unsigned int seed) {

    std::vector<int> permutation(256);
    iota(permutation.begin(), permutation.end(), 0);

    std::mt19937 rng(seed);
    std::shuffle(permutation.begin(), permutation.end(), rng);

    for (int i = 0; i < 256; i++)
    {
        p[i] = permutation[i];
        p[i + 256] = permutation[i];
    }
}

double NoiseGenerator::PerlinNoise(double x, double y)
{
    int xi = (int)floor(x) & 255;
    int yi = (int)floor(y) & 255;

    double xf = x - floor(x);
    double yf = y - floor(y);

    double u = fade(xf);
    double v = fade(yf);

    int topLeft = p[p[xi] + yi];
    int topRight = p[p[xi + 1] + yi];
    int bottomLeft = p[p[xi] + yi + 1];
    int bottomRight = p[p[xi + 1] + yi + 1];

    double dotTopLeft = grad(topLeft, xf, yf);
    double dotTopRight = grad(topRight, xf - 1, yf);
    double dotBottomLeft = grad(bottomLeft, xf, yf - 1);
    double dotBottomRight = grad(bottomRight, xf - 1, yf - 1);

    double lerpTop = lerp(dotTopLeft, dotTopRight, u);
    double lerpBottom = lerp(dotBottomLeft, dotBottomRight, u);

    double result = lerp(lerpTop, lerpBottom, v);

    return result;
}

double NoiseGenerator::FractalNoise(double x, double y, int octaves, double lacunarity, double persistence)
{
    double total = 0;
    double frequency = 1;
    double amplitude = 1;
    double maxValue = 0;

    for (int i = 0; i < octaves; i++)
    {
        total += PerlinNoise(x * frequency, y * frequency);
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return total / maxValue;
}