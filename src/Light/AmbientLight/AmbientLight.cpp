// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbientLight implementation
*/

#include "Light/AmbientLight/AmbientLight.hpp"

namespace RayTracer {
AmbientLight::AmbientLight() {
    const Math::Coords colors{0.1, 0.1, 0.1};
    color = Math::Vector3D(colors);
}

AmbientLight::AmbientLight(const Math::Vector3D &colors) : color(colors) {}

void AmbientLight::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& colorSetting = setting->add("color", libconfig::Setting::TypeGroup);
    colorSetting.add("r", libconfig::Setting::TypeFloat) = color.X;
    colorSetting.add("g", libconfig::Setting::TypeFloat) = color.Y;
    colorSetting.add("b", libconfig::Setting::TypeFloat) = color.Z;
}

Math::Vector3D AmbientLight::getLightColor() const {
    return color;
}

}  // namespace RayTracer
