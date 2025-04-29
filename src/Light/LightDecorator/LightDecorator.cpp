// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightDecorator implementation
*/
#include <memory>
#include <utility>
#include "LightDecorator.hpp"

namespace RayTracer {

LightDecorator::LightDecorator(std::shared_ptr<ILight> light)
: wrappedLight(std::move(light)) {}

Math::Vector3D LightDecorator::getLightDirection(
const Math::Point3D &point) const {
    return wrappedLight->getLightDirection(point);
}

Math::Vector3D LightDecorator::getLightColor() const {
    return wrappedLight->getLightColor();
}

void LightDecorator::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    wrappedLight->getLibConfigParams(setting);
}

}  // namespace RayTracer
