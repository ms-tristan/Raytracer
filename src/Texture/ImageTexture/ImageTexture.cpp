// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ImageTexture implementation
*/

#define STB_IMAGE_IMPLEMENTATION
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../../../tests/external/stb_image.h"
#include "ImageTexture.hpp"

namespace RayTracer {

ImageTexture::ImageTexture(const std::string& filepath, bool useBilinearFilter)
: ATexture("image"), bilinearFilter(useBilinearFilter), width(0), height(0), channels(0) {
    loadImageData(filepath);
}

void ImageTexture::loadImageData(const std::string& filepath) {
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        createDefaultTexture();
        return;
    }
    storeImageData(data);
}

void ImageTexture::createDefaultTexture() {
    std::cerr << "ERROR: Unable to load texture from the specified path" << std::endl;
    width = 2;
    height = 2;
    channels = 3;
    imageData.resize(width * height * channels);
    // First pixel (red)
    imageData[0] = 255; // R
    imageData[1] = 0;   // G
    imageData[2] = 0;   // B
    // Second pixel (white)
    imageData[3] = 255; // R
    imageData[4] = 255; // G
    imageData[5] = 255; // B
    // Third pixel (white)
    imageData[6] = 255; // R
    imageData[7] = 255; // G
    imageData[8] = 255; // B
    // Fourth pixel (red)
    imageData[9] = 255; // R
    imageData[10] = 0;  // G
    imageData[11] = 0;  // B
}

void ImageTexture::storeImageData(unsigned char* data) {
    imageData.resize(width * height * channels);
    std::copy(data, data + (width * height * channels), imageData.begin());
    stbi_image_free(data);
}

Math::Vector3D ImageTexture::getColorAt(const Math::Vector2D& uv) const {
    double u = normalizeCoordinate(uv.U);
    double v = normalizeCoordinate(uv.V);

    v = 1.0 - v;
    return bilinearFilter ? bilinearSample(u, v) : nearestNeighbor(u, v);
}

double ImageTexture::normalizeCoordinate(double coord) const {
    double result = std::fmod(coord, 1.0);
    if (result < 0) {
        result += 1.0;
    }
    return result;
}

Math::Vector3D ImageTexture::nearestNeighbor(double u, double v) const {
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    x = std::clamp(x, 0, width - 1);
    y = std::clamp(y, 0, height - 1);
    size_t idx = (y * width + x) * channels;
    double r = imageData[idx] / 255.0;
    double g = channels > 1 ? imageData[idx + 1] / 255.0 : r;
    double b = channels > 2 ? imageData[idx + 2] / 255.0 : g;

    return Math::Vector3D(Math::Coords{r, g, b});
}

Math::Vector3D ImageTexture::bilinearSample(double u, double v) const {
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
    double r = interpolateChannel(0, tx, ty, idx00, idx01, idx10, idx11);
    double g = channels > 1 ?
        interpolateChannel(1, tx, ty, idx00, idx01, idx10, idx11) : r;
    double b = channels > 2 ?
        interpolateChannel(2, tx, ty, idx00, idx01, idx10, idx11) : g;

    return Math::Vector3D(Math::Coords{r, g, b});
}

double ImageTexture::interpolateChannel(int channelOffset, double tx, double ty,
                                        size_t idx00, size_t idx01,
                                        size_t idx10, size_t idx11) const {
    double v00 = imageData[idx00 + channelOffset] / 255.0;
    double v01 = imageData[idx01 + channelOffset] / 255.0;
    double v10 = imageData[idx10 + channelOffset] / 255.0;
    double v11 = imageData[idx11 + channelOffset] / 255.0;

    return (1 - tx) * (1 - ty) * v00 +
           tx * (1 - ty) * v01 +
           (1 - tx) * ty * v10 +
           tx * ty * v11;
}

std::shared_ptr<ITexture> ImageTexture::clone() const {
    auto copy = std::make_shared<ImageTexture>("", bilinearFilter);

    copy->width = width;
    copy->height = height;
    copy->channels = channels;
    copy->imageData = imageData;
    return copy;
}

}  // namespace RayTracer
