// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BlurPostProcess implementation
*/

#include <memory>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <numeric>
#include "BlurPostProcess.hpp"

namespace RayTracer {

BlurPostProcess::BlurPostProcess(double radius)
    : radius(radius) {
}

std::vector<Math::Vector3D> BlurPostProcess::processFrameBuffer(
  const std::vector<Math::Vector3D>& frameBuffer,
  int width, int height) const {
  std::vector<Math::Vector3D> result(frameBuffer.size());

  int kernelSize = static_cast<int>(radius * 2 + 1);
  double sigma = radius / 3.0;
  double twoSigmaSquared = 2.0 * sigma * sigma;

  for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
          int pixelIndex = y * width + x;

          double sumR = 0.0;
          double sumG = 0.0;
          double sumB = 0.0;
          double weightSum = 0.0;

          for (int ky = -kernelSize/2; ky <= kernelSize/2; ++ky) {
              for (int kx = -kernelSize/2; kx <= kernelSize/2; ++kx) {
                  int sampleX = x + kx;
                  int sampleY = y + ky;

                  if (sampleX < 0 || sampleX >= width || sampleY < 0 || sampleY >= height) {
                      continue;
                  }

                  double distance = kx*kx + ky*ky;

                  double weight = std::exp(-distance / twoSigmaSquared);

                  int sampleIndex = sampleY * width + sampleX;
                  const Math::Vector3D& sampleColor = frameBuffer[sampleIndex];

                  sumR += sampleColor.X * weight;
                  sumG += sampleColor.Y * weight;
                  sumB += sampleColor.Z * weight;
                  weightSum += weight;
              }
          }

          if (weightSum > 0) {
              result[pixelIndex] = Math::Vector3D(
                  Math::Coords{sumR / weightSum, sumG / weightSum, sumB / weightSum}
              );
          } else {
              result[pixelIndex] = frameBuffer[pixelIndex];
          }
      }
  }

  return result;
}

std::string BlurPostProcess::getTypeName() const {
    return getTypeNameStatic();
}

std::string BlurPostProcess::getTypeNameStatic() {
    return "blur";
}

std::shared_ptr<IPostProcess> BlurPostProcess::clone() const {
    return std::make_shared<BlurPostProcess>(radius);
}

void BlurPostProcess::getLibConfigParams(libconfig::Setting& setting) const {
    setting.add("type", libconfig::Setting::TypeString) = getTypeName();
    setting.add("radius", libconfig::Setting::TypeFloat) = radius;
}

}  // namespace RayTracer
