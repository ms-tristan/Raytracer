// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rotation
*/
#include "Rotation.hpp"
#include <cmath>

namespace Math {
Vector3D Rotation::rotateX(const Vector3D &v, double degrees) {
    double rad = toRadians(degrees);
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);
    const Coords coords{v.X, v.Y * cosA - v.Z * sinA, v.Y * sinA + v.Z * cosA};

    return Vector3D(coords);
}

Vector3D Rotation::rotateY(const Vector3D &v, double degrees) {
    double rad = toRadians(degrees);
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);
    const Coords coords{v.X * cosA + v.Z * sinA, v.Y, -v.X * sinA + v.Z * cosA};

    return Vector3D(coords);
}

Vector3D Rotation::rotateZ(const Vector3D &v, double degrees) {
    double rad = toRadians(degrees);
    double cosA = std::cos(rad);
    double sinA = std::sin(rad);
    const Coords coords{v.X * cosA - v.Y * sinA, v.X * sinA + v.Y * cosA, v.Z};

    return Vector3D(coords);
}

Point3D Rotation::rotateX(const Point3D &p, double degrees) {
    const Coords coords_vec{p.X, p.Y, p.Z};
    Vector3D v = rotateX(Vector3D(coords_vec), degrees);
    const Coords coords_point{v.X, v.Y, v.Z};
    return Point3D(coords_point);
}

Point3D Rotation::rotateY(const Point3D &p, double degrees) {
    const Coords coords_vec{p.X, p.Y, p.Z};
    Vector3D v = rotateY(Vector3D(coords_vec), degrees);
    const Coords coords_point{v.X, v.Y, v.Z};
    return Point3D(coords_point);
}

Point3D Rotation::rotateZ(const Point3D &p, double degrees) {
    const Coords coords_vec{p.X, p.Y, p.Z};
    Vector3D v = rotateZ(Vector3D(coords_vec), degrees);
    const Coords coords_point{v.X, v.Y, v.Z};
    return Point3D(coords_point);
}
} // namespace Math
