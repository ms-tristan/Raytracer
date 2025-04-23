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
}  // namespace RayTracer
