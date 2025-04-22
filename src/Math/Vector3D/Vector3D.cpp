// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector3D
*/
#include "Vector3D/Vector3D.hpp"

namespace Math {

Vector3D::Vector3D(const Coords &coords)
    : X(coords.Xcoords), Y(coords.Ycoords), Z(coords.Zcoords) {}

double Vector3D::length() const { return std::sqrt(X * X + Y * Y + Z * Z); }

Vector3D Vector3D::normalize() const {
    double len = length();
    if (len > 0) {
        const Coords coords{X / len, Y / len, Z / len};
        return Vector3D(coords);
    }
    return *this;
}

Vector3D Vector3D::cross(const Vector3D &other) const {
    const Coords coords{Y * other.Z - Z * other.Y, Z * other.X - X * other.Z,
                        X * other.Y - Y * other.X};
    return Vector3D(coords);
}

Vector3D Vector3D::operator+(const Vector3D &other) const {
    const Coords coords{X + other.X, Y + other.Y, Z + other.Z};
    return Vector3D(coords);
}

Vector3D &Vector3D::operator+=(const Vector3D &other) {
    X += other.X;
    Y += other.Y;
    Z += other.Z;
    return *this;
}

Vector3D Vector3D::operator-(const Vector3D &other) const {
    const Coords coords{X - other.X, Y - other.Y, Z - other.Z};
    return Vector3D(coords);
}

Vector3D &Vector3D::operator-=(const Vector3D &other) {
    X -= other.X;
    Y -= other.Y;
    Z -= other.Z;
    return *this;
}

Vector3D Vector3D::operator*(const Vector3D &other) const {
    const Coords coords{X * other.X, Y * other.Y, Z * other.Z};
    return Vector3D(coords);
}

Vector3D &Vector3D::operator*=(const Vector3D &other) {
    X *= other.X;
    Y *= other.Y;
    Z *= other.Z;
    return *this;
}

Vector3D Vector3D::operator/(const Vector3D &other) const {
    const Coords coords{X / other.X, Y / other.Y, Z / other.Z};
    return Vector3D(coords);
}

Vector3D &Vector3D::operator/=(const Vector3D &other) {
    X /= other.X;
    Y /= other.Y;
    Z /= other.Z;
    return *this;
}

Vector3D Vector3D::operator*(double scalar) const {
    const Coords coords{X * scalar, Y * scalar, Z * scalar};
    return Vector3D(coords);
}

Vector3D &Vector3D::operator*=(double scalar) {
    X *= scalar;
    Y *= scalar;
    Z *= scalar;
    return *this;
}

Vector3D Vector3D::operator/(double scalar) const {
    const Coords coords{X / scalar, Y / scalar, Z / scalar};
    return Vector3D(coords);
}

Vector3D &Vector3D::operator/=(double scalar) {
    X /= scalar;
    Y /= scalar;
    Z /= scalar;
    return *this;
}

double Vector3D::dot(const Vector3D &other) const {
    return X * other.X + Y * other.Y + Z * other.Z;
}
} // namespace Math
