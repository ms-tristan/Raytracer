// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rectangle3D
*/
#include "Rectangle3D/Rectangle3D.hpp"

namespace RayTracer {
Rectangle3D::Rectangle3D(const Math::Point3D &origin, const Math::Vector3D &bottom_side,
                         const Math::Vector3D &left_side)
    : origin(origin), bottom_side(bottom_side), left_side(left_side) {}

Math::Point3D Rectangle3D::pointAt(double u, double v) const {
    return origin + bottom_side * u + left_side * v;
}
} // namespace RayTracer
