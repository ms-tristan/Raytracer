// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#ifndef SRC_MATERIAL_MATERIAL_HPP_
#define SRC_MATERIAL_MATERIAL_HPP_
#include <algorithm>
#include "defs.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {
class Material {
 public:
    Math::Vector3D color;
    double reflectivity;
    double transparency;
    double refractionIndex;

    Material();
    explicit Material(const Math::Vector3D &color, double reflectivity = 0.0,
        double transparency = 0.0, double refractionIndex = 1.0);
    virtual ~Material() = default;
};
}  // namespace RayTracer

#endif  // SRC_MATERIAL_MATERIAL_HPP_
