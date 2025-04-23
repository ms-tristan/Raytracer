// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Point3D
*/

#ifndef SRC_MATH_POINT3D_POINT3D_HPP_
#define SRC_MATH_POINT3D_POINT3D_HPP_
#include "Math/Vector3D/Vector3D.hpp"
namespace Math {
class Point3D {
 public:
    double X;
    double Y;
    double Z;

    Point3D() = default;
    explicit Point3D(const Coords &coords);

    Point3D operator+(const Vector3D &vector) const;
    Point3D &operator+=(const Vector3D &vector);
    Point3D operator-(const Vector3D &vector) const;
    Point3D &operator-=(const Vector3D &vector);
    Vector3D operator-(const Point3D &other) const;
};
}  // namespace Math

#endif  // SRC_MATH_POINT3D_POINT3D_HPP_
