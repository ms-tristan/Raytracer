// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rotate transformation implementation
*/
#include <stdexcept>
#include <string>
#include "Transformation/Rotate/Rotate.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Point3D/Point3D.hpp"


namespace RayTracer {

Rotate::Rotate(const std::string& axis, double angle)
: axis(axis), angle(angle) {
    if (axis != "x" && axis != "y" && axis != "z")
        throw std::invalid_argument("Rotation axis must be x, y, or z");
}

Math::Vector3D Rotate::applyToVector(const Math::Vector3D& vector) const {
    double radians = angle * M_PI / 180.0;
    double cosA = std::cos(radians);
    double sinA = std::sin(radians);

    if (axis == "x") {
        return Math::Vector3D(Math::Coords{
            vector.X,
            vector.Y * cosA - vector.Z * sinA,
            vector.Y * sinA + vector.Z * cosA
        });
    } else if (axis == "y") {
        return Math::Vector3D(Math::Coords{
            vector.X * cosA + vector.Z * sinA,
            vector.Y,
            -vector.X * sinA + vector.Z * cosA
        });
    } else {
        return Math::Vector3D(Math::Coords{
            vector.X * cosA - vector.Y * sinA,
            vector.X * sinA + vector.Y * cosA,
            vector.Z
        });
    }
}

Math::Point3D Rotate::applyToPoint(const Math::Point3D& point) const {
    double radians = angle * M_PI / 180.0;
    double cosA = std::cos(radians);
    double sinA = std::sin(radians);

    if (axis == "x") {
        return Math::Point3D(Math::Coords{
            point.X,
            point.Y * cosA - point.Z * sinA,
            point.Y * sinA + point.Z * cosA
        });
    } else if (axis == "y") {
        return Math::Point3D(Math::Coords{
            point.X * cosA + point.Z * sinA,
            point.Y,
            -point.X * sinA + point.Z * cosA
        });
    } else {
        return Math::Point3D(Math::Coords{
            point.X * cosA - point.Y * sinA,
            point.X * sinA + point.Y * cosA,
            point.Z
        });
    }
}


}  // namespace RayTracer
