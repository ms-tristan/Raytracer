// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight implementation
*/
#include <memory>
#include "Light/DirectionalLight/DirectionalLight.hpp"

namespace RayTracer {
DirectionalLight::DirectionalLight(const Math::Vector3D &direction,
const Math::Vector3D &color)
: ALight(color), direction(direction.normalize()) {}

Math::Vector3D DirectionalLight::getLightDirection(
const Math::Point3D &) const {
    return direction * -1.0;
}

std::shared_ptr<ILight> DirectionalLight::clone() const {
    return std::make_shared<DirectionalLight>(direction, color);
}

void DirectionalLight::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& dir = setting->add("direction", libconfig::Setting::TypeGroup);
    dir.add("x", libconfig::Setting::TypeFloat) = direction.X;
    dir.add("y", libconfig::Setting::TypeFloat) = direction.Y;
    dir.add("z", libconfig::Setting::TypeFloat) = direction.Z;

    libconfig::Setting& colorSetting = setting->add("color", libconfig::Setting::TypeGroup);
    colorSetting.add("r", libconfig::Setting::TypeFloat) = color.X;
    colorSetting.add("g", libconfig::Setting::TypeFloat) = color.Y;
    colorSetting.add("b", libconfig::Setting::TypeFloat) = color.Z;
}

}  // namespace RayTracer
