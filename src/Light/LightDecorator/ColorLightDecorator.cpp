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

}  // namespace RayTracer
