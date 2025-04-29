// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ColorLightDecorator implementation
*/
#include <memory>
#include <utility>
#include "ColorLightDecorator.hpp"

namespace RayTracer {

ColorLightDecorator::ColorLightDecorator(std::shared_ptr<ILight> light,
const Math::Vector3D& filter)
: LightDecorator(std::move(light)), colorFilter(filter) {}

Math::Vector3D ColorLightDecorator::getLightColor() const {
    Math::Vector3D baseColor = wrappedLight->getLightColor();
    return Math::Vector3D(Math::Coords{
        baseColor.X * colorFilter.X,
        baseColor.Y * colorFilter.Y,
        baseColor.Z * colorFilter.Z
    });
}

std::shared_ptr<ILight> ColorLightDecorator::clone() const {
    return std::make_shared<ColorLightDecorator>(
        wrappedLight->clone(), colorFilter);
}

void ColorLightDecorator::getLibConfigParams(libconfig::Setting& setting) const {
    LightDecorator::getLibConfigParams(setting);

    if (setting.exists("color")) {
        libconfig::Setting& color = setting["color"];
        color["r"] = static_cast<double>(color["r"]) * colorFilter.X;
        color["g"] = static_cast<double>(color["g"]) * colorFilter.Y;
        color["b"] = static_cast<double>(color["b"]) * colorFilter.Z;
    } else {
        libconfig::Setting& filter = setting.add("colorFilter", libconfig::Setting::TypeGroup);
        filter.add("r", libconfig::Setting::TypeFloat) = colorFilter.X;
        filter.add("g", libconfig::Setting::TypeFloat) = colorFilter.Y;
        filter.add("b", libconfig::Setting::TypeFloat) = colorFilter.Z;
    }
}

}  // namespace RayTracer
