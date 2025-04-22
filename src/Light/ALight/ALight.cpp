// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ALight implementation
*/
#include "Light/ALight/ALight.hpp"

namespace RayTracer {
ALight::ALight() {
    const Math::Coords colors{1.0, 1.0, 1.0};
    color = Math::Vector3D(colors);
}

ALight::ALight(const Math::Vector3D &colors) : color(colors) {}

Math::Vector3D ALight::getLightColor() const { return color; }
} // namespace RayTracer
