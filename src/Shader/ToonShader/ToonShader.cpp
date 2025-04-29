// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ToonShader implementation
*/
#include <memory>
#include <cmath>
#include <algorithm>
#include "ToonShader.hpp"

namespace RayTracer {

ToonShader::ToonShader(int levels, double edgeThreshold)
    : AShader("toon"), levels(levels), edgeThreshold(edgeThreshold) {}

Math::Vector3D ToonShader::apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const {
    Math::Vector3D viewDir = (ray.origin - hitInfo.hitPoint).normalize();
    double edgeFactor = std::abs(hitInfo.normal.dot(viewDir));

    if (edgeFactor < edgeThreshold)
        return Math::Vector3D(Math::Coords{0.0, 0.0, 0.0});

    double step = 1.0 / levels;

    double r = std::floor(color.X / step) * step;
    double g = std::floor(color.Y / step) * step;
    double b = std::floor(color.Z / step) * step;

    return Math::Vector3D(Math::Coords{r, g, b});
}

std::shared_ptr<IShader> ToonShader::clone() const {
    return std::make_shared<ToonShader>(levels, edgeThreshold);
}

void ToonShader::getLibConfigParams(libconfig::Setting& setting) const {
    AShader::getLibConfigParams(setting);
    setting.add("levels", libconfig::Setting::TypeInt) = levels;
    setting.add("edgeThreshold", libconfig::Setting::TypeFloat) = edgeThreshold;
}

}  // namespace RayTracer
