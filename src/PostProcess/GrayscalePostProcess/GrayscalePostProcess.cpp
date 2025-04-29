// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** GrayscalePostProcess implementation
*/

#include <memory>
#include <string>
#include <vector>
#include "GrayscalePostProcess.hpp"

namespace RayTracer {

GrayscalePostProcess::GrayscalePostProcess(double intensity)
    : intensity(intensity) {
}

std::vector<Math::Vector3D> GrayscalePostProcess::processFrameBuffer(
    const std::vector<Math::Vector3D>& frameBuffer,
    int width, int height) const {

    std::vector<Math::Vector3D> result(frameBuffer.size());

    for (size_t i = 0; i < frameBuffer.size(); ++i) {
        const Math::Vector3D& color = frameBuffer[i];

        double grayValue = 0.299 * color.X + 0.587 * color.Y + 0.114 * color.Z;

        double r = color.X * (1.0 - intensity) + grayValue * intensity;
        double g = color.Y * (1.0 - intensity) + grayValue * intensity;
        double b = color.Z * (1.0 - intensity) + grayValue * intensity;

        result[i] = Math::Vector3D(Math::Coords{r, g, b});
    }

    return result;
}

std::string GrayscalePostProcess::getTypeName() const {
    return getTypeNameStatic();
}

std::string GrayscalePostProcess::getTypeNameStatic() {
    return "grayscale";
}

std::shared_ptr<IPostProcess> GrayscalePostProcess::clone() const {
    return std::make_shared<GrayscalePostProcess>(intensity);
}

void GrayscalePostProcess::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    setting->add("type", libconfig::Setting::TypeString) = getTypeName();
    setting->add("intensity", libconfig::Setting::TypeFloat) = intensity;
}

}  // namespace RayTracer
