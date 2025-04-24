// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PointLight implementation
*/

#include "Light/PointLight/PointLight.hpp"
#include <memory>

namespace RayTracer {
PointLight::PointLight(const Math::Point3D &position,
const Math::Vector3D &color, double constant, double linear, double quadratic)
: ALight(color), position(position), constantAttenuation(constant),
linearAttenuation(linear), quadraticAttenuation(quadratic) {}

Math::Vector3D PointLight::getLightDirection(const Math::Point3D &point) const {
    Math::Vector3D direction = position - point;
    return direction.normalize();
}

Math::Vector3D PointLight::getLightColor(const Math::Point3D &point) const {
    Math::Vector3D distanceVector = position - point;
    double distance = distanceVector.length();

    double attenuation = 1.0 / (constantAttenuation +
        linearAttenuation * distance +
        quadraticAttenuation * distance * distance);

    return color * attenuation;
}

std::shared_ptr<ILight> PointLight::clone() const {
    return std::make_shared<PointLight>(
        position, color, constantAttenuation,
        linearAttenuation, quadraticAttenuation);
}
}  // namespace RayTracer
