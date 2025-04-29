// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scale transformation implementation
*/

#include "Transformation/Scale/Scale.hpp"

namespace RayTracer {

Scale::Scale(double factor) : factor(factor) {}

Math::Vector3D Scale::applyToVector(const Math::Vector3D& vector) const {
    return vector * factor;
}

Math::Point3D Scale::applyToPoint(const Math::Point3D& point) const {
    Math::Coords coords{
        point.X * factor,
        point.Y * factor,
        point.Z * factor
    };
    return Math::Point3D(coords);
}

}  // namespace RayTracer
