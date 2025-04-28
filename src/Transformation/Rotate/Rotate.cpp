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

Math::Vector3D Rotate::applyToVector(const Math::Vector3D& v) const {
    double radians = angle * M_PI / 180.0;
    double cosA = std::cos(radians);
    double sinA = std::sin(radians);

    if (axis == "x") {
        double rad = toRadians(angle);
        double cosA = std::cos(rad);
        double sinA = std::sin(rad);
        const Math::Coords coords{v.X, v.Y * cosA - v.Z * sinA, v.Y * sinA + v.Z * cosA};

        return Math::Vector3D(coords);

    } else if (axis == "y") {
        double rad = toRadians(angle);
        double cosA = std::cos(rad);
        double sinA = std::sin(rad);
        const Math::Coords coords{v.X * cosA + v.Z * sinA, v.Y, v.Z * cosA - v.X * sinA};

        return Math::Vector3D(coords);
    } else {
        double rad = toRadians(angle);
        double cosA = std::cos(rad);
        double sinA = std::sin(rad);
        const Math::Coords coords{v.X * cosA - v.Y * sinA, v.X * sinA + v.Y * cosA, v.Z};

        return Math::Vector3D(coords);
    }
}

Math::Point3D Rotate::applyToPoint(const Math::Point3D& p) const {
    double radians = angle * M_PI / 180.0;
    double cosA = std::cos(radians);
    double sinA = std::sin(radians);

    if (axis == "x") {
        const Math::Coords coords_vec{p.X, p.Y, p.Z};
        Math::Vector3D v = Rotate("x", angle).applyToVector(Math::Vector3D(coords_vec));
        const Math::Coords coords_point{v.X, v.Y, v.Z};
        return Math::Point3D(coords_point);
    } else if (axis == "y") {
        const Math::Coords coords_vec{p.X, p.Y, p.Z};
        Math::Vector3D v = Rotate("y", angle).applyToVector(Math::Vector3D(coords_vec));
        const Math::Coords coords_point{v.X, v.Y, v.Z};
        return Math::Point3D(coords_point);
    } else {
        const Math::Coords coords_vec{p.X, p.Y, p.Z};
        Math::Vector3D v = Rotate("z", angle).applyToVector(Math::Vector3D(coords_vec));
        const Math::Coords coords_point{v.X, v.Y, v.Z};
        return Math::Point3D(coords_point);
    }
}


}  // namespace RayTracer
