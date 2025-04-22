// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rectangle3D
*/

#ifndef SRC_MATH_RECTANGLE3D_RECTANGLE3D_HPP_
#define SRC_MATH_RECTANGLE3D_RECTANGLE3D_HPP_
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {
class Rectangle3D {
  public:
    Math::Point3D origin;
    Math::Vector3D bottom_side;
    Math::Vector3D left_side;

    Rectangle3D() = default;
    Rectangle3D(const Math::Point3D &origin, const Math::Vector3D &bottom_side,
                const Math::Vector3D &left_side);
    Math::Point3D pointAt(double u, double v) const;
};
} // namespace RayTracer

#endif // SRC_MATH_RECTANGLE3D_RECTANGLE3D_HPP_
