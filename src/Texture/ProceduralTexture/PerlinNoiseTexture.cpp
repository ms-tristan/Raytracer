// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PerlinNoiseTexture implementation
*/

#include <cmath>
#include <algorithm>
#include <random>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "PerlinNoiseTexture.hpp"

namespace RayTracer {

PerlinNoiseTexture::PerlinNoiseTexture(const Math::Vector3D& color1,
                                       const Math::Vector3D& color2,
                                       double scale,
                                       double persistence,
                                       int octaves,
                                       unsigned int seed)
: ProceduralTexture("perlin", color1, color2, scale),
  persistence(persistence),
  octaves(octaves) {
    initPermutation(seed);
}

void PerlinNoiseTexture::initPermutation(unsigned int seed) {
    perm.resize(PERM_SIZE * 2);
    std::vector<int> p(PERM_SIZE);
    for (int i = 0; i < PERM_SIZE; i++)
        p[i] = i;
    std::mt19937 gen(seed);
    std::shuffle(p.begin(), p.end(), gen);
    for (int i = 0; i < PERM_SIZE; i++)
        perm[i] = perm[i + PERM_SIZE] = p[i];
}

double PerlinNoiseTexture::fade(double t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoiseTexture::lerp(double t, double a, double b) const {
    return a + t * (b - a);
}

double PerlinNoiseTexture::grad(int hash, double x, double y, double z) const {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);

    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double PerlinNoiseTexture::noise(double x, double y, double z) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    int Z = static_cast<int>(std::floor(z)) & 255;
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);
    int A = perm[X] + Y;
    int AA = perm[A] + Z;
    int AB = perm[A + 1] + Z;
    int B = perm[X + 1] + Y;
    int BA = perm[B] + Z;
    int BB = perm[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(perm[AA], x, y, z),
                                  grad(perm[BA], x - 1, y, z)),
                          lerp(u, grad(perm[AB], x, y - 1, z),
                                  grad(perm[BB], x - 1, y - 1, z))),
                  lerp(v, lerp(u, grad(perm[AA + 1], x, y, z - 1),
                                  grad(perm[BA + 1], x - 1, y, z - 1)),
                          lerp(u, grad(perm[AB + 1], x, y - 1, z - 1),
                                  grad(perm[BB + 1], x - 1, y - 1, z - 1))));
}

double PerlinNoiseTexture::turbulence(double x, double y, double z, int depth) const {
    double total = 0.0;
    double freq = 1.0;
    double amp = 1.0;
    double maxValue = 0.0;
    for (int i = 0; i < depth; i++) {
        total += std::abs(noise(x * freq, y * freq, z * freq)) * amp;
        maxValue += amp;
        amp *= persistence;
        freq *= 2.0;
    }
    return total / maxValue;
}

Math::Vector3D PerlinNoiseTexture::getColorAt(const Math::Vector2D& uv) const {
    double noiseValue = turbulence(uv.U * scale, uv.V * scale, 0.0, octaves);

    return color1 * (1.0 - noiseValue) + color2 * noiseValue;
}

std::shared_ptr<ITexture> PerlinNoiseTexture::clone() const {
    auto copy = std::make_shared<PerlinNoiseTexture>(color1, color2, scale,
        persistence, octaves);
    copy->perm = perm;
    return copy;
}

double PerlinNoiseTexture::getPersistence() const {
    return persistence;
}

int PerlinNoiseTexture::getOctaves() const {
    return octaves;
}

void PerlinNoiseTexture::setPersistence(double value) {
    persistence = value;
}

void PerlinNoiseTexture::setOctaves(int value) {
    octaves = value;
}

}  // namespace RayTracer
