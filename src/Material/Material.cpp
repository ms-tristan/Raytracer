// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/
#include "Material/Material.hpp"

namespace RayTracer {
Material::Material() {
    const Math::Coords colors{1.0, 1.0, 1.0};
    color = Math::Vector3D(colors);
}

Material::Material(const Math::Vector3D &colors) : color(colors) {}

} // namespace RayTracer
