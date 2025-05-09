// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DisplacementMap implementation - optimized version
*/

#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "../../../tests/external/stb_image.h"
#include "DisplacementMap.hpp"

namespace RayTracer {

DisplacementMap::DisplacementMap(const std::string& filepath, double strength, bool useBilinearFilter)
: strength(strength), bilinearFilter(useBilinearFilter), width(0), height(0), channels(0) {
    loadMapData(filepath);
}

void DisplacementMap::loadMapData(const std::string& filepath) {
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        createDefaultMap();
        return;
    }
    storeMapData(data);
    std::cout << "Displacement map loaded: " << width << "x" << height
              << " with " << channels << " channels" << std::endl;
}

void DisplacementMap::createDefaultMap() {
    std::cerr << "ERROR: Unable to load displacement map from " << filepath << std::endl;
    width = 1;
    height = 1;
    channels = 1;
    mapData.resize(width * height * channels);
    mapData[0] = 0;
}

void DisplacementMap::storeMapData(unsigned char* data) {
    mapData.resize(width * height * channels);
    std::copy(data, data + (width * height * channels), mapData.begin());
    stbi_image_free(data);
}

double DisplacementMap::getDisplacementAt(const Math::Vector2D& uv) const {
    double u = normalizeCoordinate(uv.U);
    double v = normalizeCoordinate(uv.V);
    v = 1.0 - v;
    double displacement = bilinearFilter ? bilinearSample(u, v) : nearestNeighbor(u, v);
    return applyEffects(displacement);
}

double DisplacementMap::normalizeCoordinate(double coord) const {
    double result = std::fmod(coord, 1.0);
    if (result < 0) {
        result += 1.0;
    }
    return result;
}

double DisplacementMap::applyEffects(double displacement) const {
    if (displacement > 0.5)
        displacement = 0.5 + 0.5 * std::pow((displacement - 0.5) * 2.0, 0.7);
    else
        displacement = 0.5 - 0.5 * std::pow((0.5 - displacement) * 2.0, 0.7);
    displacement = std::clamp(displacement, 0.0, 1.0);
    return displacement * strength;
}

double DisplacementMap::nearestNeighbor(double u, double v) const {
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);

    x = std::clamp(x, 0, width - 1);
    y = std::clamp(y, 0, height - 1);
    size_t idx = (y * width + x) * channels;
    return mapData[idx] / 255.0;
}

double DisplacementMap::bilinearSample(double u, double v) const {
    double x = u * (width - 1);
    double y = v * (height - 1);
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int x1 = std::min(x0 + 1, width - 1);
    int y1 = std::min(y0 + 1, height - 1);
    double tx = x - x0;
    double ty = y - y0;
    size_t idx00 = (y0 * width + x0) * channels;
    size_t idx01 = (y0 * width + x1) * channels;
    size_t idx10 = (y1 * width + x0) * channels;
    size_t idx11 = (y1 * width + x1) * channels;
    double v00 = mapData[idx00] / 255.0;
    double v01 = mapData[idx01] / 255.0;
    double v10 = mapData[idx10] / 255.0;
    double v11 = mapData[idx11] / 255.0;

    return interpolateValues(tx, ty, v00, v01, v10, v11);
}

double DisplacementMap::interpolateValues(double tx, double ty, double v00, double v01, double v10, double v11) const {
    return (1 - tx) * (1 - ty) * v00 +
           tx * (1 - ty) * v01 +
           (1 - tx) * ty * v10 +
           tx * ty * v11;
}

std::shared_ptr<DisplacementMap> DisplacementMap::clone() const {
    auto copy = std::make_shared<DisplacementMap>("", strength, bilinearFilter);
    copy->width = width;
    copy->height = height;
    copy->channels = channels;
    copy->mapData = mapData;
    return copy;
}

double DisplacementMap::getStrength() const {
    return strength;
}

void DisplacementMap::setStrength(double value) {
    strength = value;
}

std::string DisplacementMap::getType() const {
    return "DisplacementMap";
}

}  // namespace RayTracer
