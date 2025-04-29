// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ChromaticAberrationPostProcess implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "ChromaticAberrationPostProcess.hpp"

namespace RayTracer {

ChromaticAberrationPostProcess::ChromaticAberrationPostProcess(double strength)
    : strength(strength) {
}

std::vector<Math::Vector3D> ChromaticAberrationPostProcess::processFrameBuffer(
    const std::vector<Math::Vector3D>& frameBuffer,
    int width, int height) const {

    std::vector<Math::Vector3D> result(frameBuffer.size());

    double centerX = width / 2.0;
    double centerY = height / 2.0;

    std::vector<Math::Vector3D> buffer = frameBuffer;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pixelIndex = y * width + x;

            double dirX = (x - centerX) / centerX;
            double dirY = (y - centerY) / centerY;

            double distance = std::sqrt(dirX * dirX + dirY * dirY);

            double offset = distance * strength;

            int redOffsetX = static_cast<int>(offset * dirX);
            int redOffsetY = static_cast<int>(offset * dirY);

            int blueOffsetX = -redOffsetX;
            int blueOffsetY = -redOffsetY;

            double r = 0.0, g = 0.0, b = 0.0;

            int redX = std::clamp(x + redOffsetX, 0, width - 1);
            int redY = std::clamp(y + redOffsetY, 0, height - 1);
            const Math::Vector3D& redSample = buffer[redY * width + redX];
            r = redSample.X;

            g = buffer[pixelIndex].Y;

            int blueX = std::clamp(x + blueOffsetX, 0, width - 1);
            int blueY = std::clamp(y + blueOffsetY, 0, height - 1);
            const Math::Vector3D& blueSample = buffer[blueY * width + blueX];
            b = blueSample.Z;

            result[pixelIndex] = Math::Vector3D(Math::Coords{r, g, b});
        }
    }

    return result;
}

std::string ChromaticAberrationPostProcess::getTypeName() const {
    return getTypeNameStatic();
}

std::string ChromaticAberrationPostProcess::getTypeNameStatic() {
    return "chromatic_aberration";
}

std::shared_ptr<IPostProcess> ChromaticAberrationPostProcess::clone() const {
    return std::make_shared<ChromaticAberrationPostProcess>(strength);
}

void ChromaticAberrationPostProcess::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    setting->add("type", libconfig::Setting::TypeString) = getTypeName();
    setting->add("strength", libconfig::Setting::TypeFloat) = strength;
}

}  // namespace RayTracer
