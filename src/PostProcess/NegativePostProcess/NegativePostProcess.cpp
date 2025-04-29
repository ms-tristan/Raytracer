// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** NegativePostProcess implementation
*/

#include <memory>
#include <string>
#include <vector>
#include "NegativePostProcess.hpp"

namespace RayTracer {

NegativePostProcess::NegativePostProcess(double intensity)
    : intensity(intensity) {
}

std::vector<Math::Vector3D> NegativePostProcess::processFrameBuffer(
    const std::vector<Math::Vector3D>& frameBuffer,
    int width, int height) const {

    std::vector<Math::Vector3D> result(frameBuffer.size());

    for (size_t i = 0; i < frameBuffer.size(); ++i) {
        const Math::Vector3D& color = frameBuffer[i];

        double r = 1.0 - color.X;
        double g = 1.0 - color.Y;
        double b = 1.0 - color.Z;

        r = color.X * (1.0 - intensity) + r * intensity;
        g = color.Y * (1.0 - intensity) + g * intensity;
        b = color.Z * (1.0 - intensity) + b * intensity;

        result[i] = Math::Vector3D(Math::Coords{r, g, b});
    }
    return result;
}

std::string NegativePostProcess::getTypeName() const {
    return getTypeNameStatic();
}

std::string NegativePostProcess::getTypeNameStatic() {
    return "negative";
}

std::shared_ptr<IPostProcess> NegativePostProcess::clone() const {
    return std::make_shared<NegativePostProcess>(intensity);
}

void NegativePostProcess::getLibConfigParams(libconfig::Setting& setting) const {
    setting.add("type", libconfig::Setting::TypeString) = getTypeName();
    setting.add("intensity", libconfig::Setting::TypeFloat) = intensity;
}

}  // namespace RayTracer
