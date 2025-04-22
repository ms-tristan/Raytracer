// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point3D
*/
#include "Point3D/Point3D.hpp"

namespace Math {

Point3D::Point3D(const Math::Coords &coords)
    : X(coords.Xcoords), Y(coords.Ycoords), Z(coords.Zcoords) {}

Point3D Point3D::operator+(const Vector3D &vector) const {
    const Coords coords{X + vector.X, Y + vector.Y, Z + vector.Z};
    return Point3D(coords);
}

Point3D &Point3D::operator+=(const Vector3D &vector) {
    X += vector.X;
    Y += vector.Y;
    Z += vector.Z;
    return *this;
}

Point3D Point3D::operator-(const Vector3D &vector) const {
    const Coords coords{X - vector.X, Y - vector.Y, Z - vector.Z};
    return Point3D(coords);
}

Point3D &Point3D::operator-=(const Vector3D &vector) {
    X -= vector.X;
    Y -= vector.Y;
    Z -= vector.Z;
    return *this;
}

Vector3D Point3D::operator-(const Point3D &other) const {
    const Coords coords{X - other.X, Y - other.Y, Z - other.Z};
    return Vector3D(coords);
}

} // namespace Math
