// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rotate transformation implementation
*/

#include "Transformation/Rotate/Rotate.hpp"
#include <stdexcept>
#include <string>

namespace RayTracer {

Rotate::Rotate(const std::string& axis, double angle)
: axis(axis), angle(angle) {
    if (axis != "x" && axis != "y" && axis != "z")
        throw std::invalid_argument("Rotation axis must be x, y, or z");
}

Math::Vector3D Rotate::applyToVector(const Math::Vector3D& vector) const {
    if (axis == "x") {
        return Math::Rotation::rotateX(vector, angle);
    } else if (axis == "y") {
        return Math::Rotation::rotateY(vector, angle);
    } else {
        return Math::Rotation::rotateZ(vector, angle);
    }
}

Math::Point3D Rotate::applyToPoint(const Math::Point3D& point) const {
    if (axis == "x") {
        return Math::Rotation::rotateX(point, angle);
    } else if (axis == "y") {
        return Math::Rotation::rotateY(point, angle);
    } else {
        return Math::Rotation::rotateZ(point, angle);
    }
}

}  // namespace RayTracer
