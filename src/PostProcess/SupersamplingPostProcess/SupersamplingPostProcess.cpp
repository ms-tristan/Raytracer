// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SupersamplingPostProcess implementation
*/

#include <memory>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <random>
#include "SupersamplingPostProcess.hpp"

namespace RayTracer {

SupersamplingPostProcess::SupersamplingPostProcess(int samplesPerPixel)
    : samplesPerPixel(samplesPerPixel) {
    if (samplesPerPixel < 1) {
        this->samplesPerPixel = 1;
    }
    int root = static_cast<int>(std::sqrt(this->samplesPerPixel));
    if (root * root != this->samplesPerPixel) {
        this->samplesPerPixel = root * root;
    }
}

std::vector<Math::Vector3D> SupersamplingPostProcess::processFrameBuffer(
    const std::vector<Math::Vector3D>& frameBuffer,
    int width, int height) const {
    return frameBuffer;
}

std::string SupersamplingPostProcess::getTypeName() const {
    return getTypeNameStatic();
}

std::string SupersamplingPostProcess::getTypeNameStatic() {
    return "supersampling";
}

std::shared_ptr<IPostProcess> SupersamplingPostProcess::clone() const {
    return std::make_shared<SupersamplingPostProcess>(samplesPerPixel);
}

void SupersamplingPostProcess::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    setting->add("samplesPerPixel", libconfig::Setting::TypeInt) = samplesPerPixel;
}

}  // namespace RayTracer
