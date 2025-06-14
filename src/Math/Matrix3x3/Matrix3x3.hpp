/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Matrix3x3 implementation
*/

#ifndef SRC_MATH_MATRIX3X3_MATRIX3X3_HPP_
#define SRC_MATH_MATRIX3X3_MATRIX3X3_HPP_

#include "Math/Vector3D/Vector3D.hpp"

namespace Math {

class Matrix3x3 {
 public:
    Matrix3x3();
    Matrix3x3(const Vector3D& row1, const Vector3D& row2, const Vector3D& row3);

    Vector3D operator*(const Vector3D& v) const;
    Matrix3x3 transpose() const;

    double m[3][3];
};

}  // namespace Math

#endif  // SRC_MATH_MATRIX3X3_MATRIX3X3_HPP_
