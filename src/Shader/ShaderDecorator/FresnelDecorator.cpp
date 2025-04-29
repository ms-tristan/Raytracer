// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** FresnelDecorator implementation
*/
#include <cmath>
#include <utility>
#include <memory>
#include <string>
#include <algorithm>
#include "FresnelDecorator.hpp"

namespace RayTracer {

FresnelDecorator::FresnelDecorator(std::shared_ptr<IShader> shader, double fresnelPower,
    const Math::Vector3D& rimColor)
    : ShaderDecorator(std::move(shader)), fresnelPower(fresnelPower), rimColor(rimColor) {}

Math::Vector3D FresnelDecorator::apply(const Math::Vector3D& color, const HitInfo& hitInfo,
    const Ray& ray) const {
    Math::Vector3D shaderResult = ShaderDecorator::apply(color, hitInfo, ray);

    Math::Vector3D viewDir = (ray.origin - hitInfo.hitPoint).normalize();
    double fresnelFactor = 1.0 - std::abs(hitInfo.normal.dot(viewDir));

    fresnelFactor = std::pow(fresnelFactor, fresnelPower);

    Math::Vector3D result = shaderResult * (1.0 - fresnelFactor) + rimColor * fresnelFactor;

    result.X = std::min(1.0, std::max(0.0, result.X));
    result.Y = std::min(1.0, std::max(0.0, result.Y));
    result.Z = std::min(1.0, std::max(0.0, result.Z));

    return result;
}

std::shared_ptr<IShader> FresnelDecorator::clone() const {
    return std::make_shared<FresnelDecorator>(wrappedShader->clone(), fresnelPower, rimColor);
}

void FresnelDecorator::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    ShaderDecorator::getLibConfigParams(setting);

    setting->add("fresnelPower", libconfig::Setting::TypeFloat) = fresnelPower;

    libconfig::Setting& color = setting->add("rimColor", libconfig::Setting::TypeGroup);
    color.add("r", libconfig::Setting::TypeFloat) = rimColor.X;
    color.add("g", libconfig::Setting::TypeFloat) = rimColor.Y;
    color.add("b", libconfig::Setting::TypeFloat) = rimColor.Z;
}

std::string FresnelDecorator::getType() const {
    return "fresnel-decorator(" + wrappedShader->getType() + ")";
}

}  // namespace RayTracer
