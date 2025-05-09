// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SpecularMap implementation
*/

#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "../../../tests/external/stb_image.h"
#include "SpecularMap.hpp"

namespace RayTracer {

SpecularMap::SpecularMap(const std::string& filepath, bool useBilinearFilter)
: bilinearFilter(useBilinearFilter), width(0), height(0), channels(0), filepath(filepath) {
    loadMapData(filepath);
}

void SpecularMap::loadMapData(const std::string& filepath) {
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        createDefaultMap();
        return;
    }
    storeMapData(data);
    std::cout << "Specular map loaded: " << width << "x" << height
              << " with " << channels << " channels" << std::endl;
}

void SpecularMap::createDefaultMap() {
    std::cerr << "ERROR: Unable to load specular map from " << filepath << std::endl;
    width = 1;
    height = 1;
    channels = 1;
    mapData.resize(width * height * channels);
    mapData[0] = 128;
}

void SpecularMap::storeMapData(unsigned char* data) {
    mapData.resize(width * height * channels);
    std::copy(data, data + (width * height * channels), mapData.begin());
    stbi_image_free(data);
}

double SpecularMap::getSpecularAt(const Math::Vector2D& uv) const {
    double u = normalizeCoordinate(uv.U);
    double v = normalizeCoordinate(uv.V);
    v = 1.0 - v;
    return bilinearFilter ? bilinearSample(u, v) : nearestNeighbor(u, v);
}

double SpecularMap::normalizeCoordinate(double coord) const {
    double result = std::fmod(coord, 1.0);
    if (result < 0) {
        result += 1.0;
    }
    return result;
}

double SpecularMap::nearestNeighbor(double u, double v) const {
    // Calculate pixel coordinates
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    x = std::clamp(x, 0, width - 1);
    y = std::clamp(y, 0, height - 1);
    size_t idx = (y * width + x) * channels;

    return mapData[idx] / 255.0;
}

double SpecularMap::bilinearSample(double u, double v) const {
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

double SpecularMap::interpolateValues(double tx, double ty, double v00,
    double v01, double v10, double v11) const {
    return (1 - tx) * (1 - ty) * v00 +
           tx * (1 - ty) * v01 +
           (1 - tx) * ty * v10 +
           tx * ty * v11;
}

std::shared_ptr<SpecularMap> SpecularMap::clone() const {
    auto copy = std::make_shared<SpecularMap>("", bilinearFilter);
    copy->width = width;
    copy->height = height;
    copy->channels = channels;
    copy->mapData = mapData;
    return copy;
}

std::string SpecularMap::getType() const {
    return "SpecularMap";
}

}  // namespace RayTracer
