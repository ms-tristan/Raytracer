// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight implementation
*/

#include "Light/DirectionalLight/DirectionalLight.hpp"

namespace RayTracer {
DirectionalLight::DirectionalLight(const Math::Vector3D &direction, const Math::Vector3D &color)
    : ALight(color), direction(direction.normalize()) {}

Math::Vector3D DirectionalLight::getLightDirection(const Math::Point3D &) const {
    return direction * -1.0;
}
} // namespace RayTracer
