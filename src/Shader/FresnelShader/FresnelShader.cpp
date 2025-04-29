// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** FresnelShader implementation
*/
#include "FresnelShader.hpp"
#include <cmath>
#include <memory>
#include <algorithm>

namespace RayTracer {

FresnelShader::FresnelShader(double fresnelPower, const Math::Vector3D& rimColor)
    : AShader("fresnel"), fresnelPower(fresnelPower), rimColor(rimColor) {}

Math::Vector3D FresnelShader::apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const {
    Math::Vector3D viewDir = (ray.origin - hitInfo.hitPoint).normalize();

    double fresnelFactor = 1.0 - std::abs(hitInfo.normal.dot(viewDir));

    fresnelFactor = std::pow(fresnelFactor, fresnelPower);

    Math::Vector3D result = color * (1.0 - fresnelFactor) + rimColor * fresnelFactor;

    result.X = std::min(1.0, std::max(0.0, result.X));
    result.Y = std::min(1.0, std::max(0.0, result.Y));
    result.Z = std::min(1.0, std::max(0.0, result.Z));

    return result;
}

std::shared_ptr<IShader> FresnelShader::clone() const {
    return std::make_shared<FresnelShader>(fresnelPower, rimColor);
}

void FresnelShader::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    AShader::getLibConfigParams(setting);

    setting->add("fresnelPower", libconfig::Setting::TypeFloat) = fresnelPower;

    libconfig::Setting& color = setting->add("rimColor", libconfig::Setting::TypeGroup);
    color.add("r", libconfig::Setting::TypeFloat) = rimColor.X;
    color.add("g", libconfig::Setting::TypeFloat) = rimColor.Y;
    color.add("b", libconfig::Setting::TypeFloat) = rimColor.Z;
}

}  // namespace RayTracer
