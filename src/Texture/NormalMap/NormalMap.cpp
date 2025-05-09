// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** NormalMap implementation
*/

#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "../../../tests/external/stb_image.h"
#include "NormalMap.hpp"

namespace RayTracer {

NormalMap::NormalMap(const std::string& filepath, double strength, bool useBilinearFilter)
: strength(strength), bilinearFilter(useBilinearFilter), width(0), height(0), channels(0), filepath(filepath) {
    loadMapData(filepath);
}

void NormalMap::loadMapData(const std::string& filepath) {
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        createDefaultNormalMap();
        return;
    }
    storeMapData(data);
    std::cout << "Normal map loaded: " << width << "x" << height
              << " with " << channels << " channels" << std::endl;
}

void NormalMap::createDefaultNormalMap() {
    std::cerr << "ERROR: Unable to load normal map from " << filepath << std::endl;
    width = 1;
    height = 1;
    channels = 3;
    mapData.resize(width * height * channels);
    mapData[0] = 128;
    mapData[1] = 128;
    mapData[2] = 255;
}

void NormalMap::storeMapData(unsigned char* data) {
    mapData.resize(width * height * channels);
    std::copy(data, data + (width * height * channels), mapData.begin());
    stbi_image_free(data);
}

Math::Vector3D NormalMap::getNormalAt(const Math::Vector2D& uv) const {
    double u = normalizeCoordinate(uv.U);
    double v = normalizeCoordinate(uv.V);
    v = 1.0 - v;
    Math::Vector3D normal = bilinearFilter ? bilinearSample(u, v) : nearestNeighbor(u, v);

    return processNormal(normal);
}

Math::Vector3D NormalMap::processNormal(Math::Vector3D normal) const {
    normal = normal * 2.0 - Math::Vector3D(Math::Coords{1.0, 1.0, 1.0});
    normal.Z = std::max(0.1, normal.Z);
    applyStrength(normal);
    return normal.normalize();
}

double NormalMap::normalizeCoordinate(double coord) const {
    double result = std::fmod(coord, 1.0);
    if (result < 0) {
        result += 1.0;
    }
    return result;
}

void NormalMap::applyStrength(Math::Vector3D& normal) const {
    if (strength != 1.0) {
        Math::Vector3D baseNormal(Math::Coords{0.0, 0.0, 1.0});
        normal = baseNormal * (1.0 - strength) + normal * strength;
    }
}

Math::Vector3D NormalMap::nearestNeighbor(double u, double v) const {
    // Calculate pixel coordinates
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    x = std::clamp(x, 0, width - 1);
    y = std::clamp(y, 0, height - 1);
    size_t idx = (y * width + x) * channels;
    return extractNormalFromIndex(idx);
}

Math::Vector3D NormalMap::extractNormalFromIndex(size_t idx) const {
    double r = mapData[idx] / 255.0;
    double g = channels > 1 ? mapData[idx + 1] / 255.0 : r;
    double b = channels > 2 ? mapData[idx + 2] / 255.0 : g;

    return Math::Vector3D(Math::Coords{r, g, b});
}

Math::Vector3D NormalMap::bilinearSample(double u, double v) const {
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
    Math::Vector3D c00 = extractNormalFromIndex(idx00);
    Math::Vector3D c01 = extractNormalFromIndex(idx01);
    Math::Vector3D c10 = extractNormalFromIndex(idx10);
    Math::Vector3D c11 = extractNormalFromIndex(idx11);

    return interpolateNormals(tx, ty, c00, c01, c10, c11);
}

Math::Vector3D NormalMap::interpolateNormals(double tx, double ty,
                                             const Math::Vector3D& c00,
                                             const Math::Vector3D& c01,
                                             const Math::Vector3D& c10,
                                             const Math::Vector3D& c11) const {
    return (c00 * ((1 - tx) * (1 - ty))) +
           (c01 * (tx * (1 - ty))) +
           (c10 * ((1 - tx) * ty)) +
           (c11 * (tx * ty));
}

std::shared_ptr<NormalMap> NormalMap::clone() const {
    auto copy = std::make_shared<NormalMap>("", strength, bilinearFilter);

    copy->width = width;
    copy->height = height;
    copy->channels = channels;
    copy->mapData = mapData;
    return copy;
}

double NormalMap::getStrength() const {
    return strength;
}

void NormalMap::setStrength(double value) {
    strength = value;
}

std::string NormalMap::getType() const {
    return "NormalMap";
}

}  // namespace RayTracer
