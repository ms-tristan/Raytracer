// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector3D
*/

#ifndef SRC_MATH_VECTOR3D_VECTOR3D_HPP_
#define SRC_MATH_VECTOR3D_VECTOR3D_HPP_
#include <cmath>

namespace Math {
struct Coords {
    double Xcoords;
    double Ycoords;
    double Zcoords;
};

class Vector3D {
 public:
    double X;
    double Y;
    double Z;

    Vector3D() = default;
    explicit Vector3D(const Coords &coords);

    double length() const;
    [[nodiscard]]Vector3D normalize() const;
    Vector3D cross(const Vector3D &other) const;
    Vector3D operator+(const Vector3D &other) const;
    Vector3D &operator+=(const Vector3D &other);
    Vector3D operator-(const Vector3D &other) const;
    Vector3D &operator-=(const Vector3D &other);
    Vector3D operator*(const Vector3D &other) const;
    Vector3D &operator*=(const Vector3D &other);
    Vector3D operator/(const Vector3D &other) const;
    Vector3D &operator/=(const Vector3D &other);
    Vector3D operator*(double scalar) const;
    Vector3D &operator*=(double scalar);
    Vector3D operator/(double scalar) const;
    Vector3D &operator/=(double scalar);
    double dot(const Vector3D &other) const;
};
inline Vector3D operator*(double scalar, const Vector3D &vector) { return vector * scalar; }
}  // namespace Math

#endif  // SRC_MATH_VECTOR3D_VECTOR3D_HPP_
