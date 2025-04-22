// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Material
*/

#ifndef SRC_MATERIAL_MATERIAL_HPP_
#define SRC_MATERIAL_MATERIAL_HPP_
#include "HitInfo.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include <algorithm>

namespace RayTracer {
class Material {
  public:
    Math::Vector3D color;

    Material();
    explicit Material(const Math::Vector3D &color);
    virtual ~Material() = default;
};
} // namespace RayTracer

#endif // SRC_MATERIAL_MATERIAL_HPP_
