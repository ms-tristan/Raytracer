// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rotation
*/

#ifndef SRC_TRANSFORMATION_ROTATION_ROTATION_HPP_
#define SRC_TRANSFORMATION_ROTATION_ROTATION_HPP_
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace Math {

inline double toRadians(double degrees) { return degrees * M_PI / 180.0; }

class Rotation {
 public:
    static Vector3D rotateX(const Vector3D &v, double degrees);
    static Vector3D rotateY(const Vector3D &v, double degrees);
    static Vector3D rotateZ(const Vector3D &v, double degrees);
    static Point3D rotateX(const Point3D &p, double degrees);
    static Point3D rotateY(const Point3D &p, double degrees);
    static Point3D rotateZ(const Point3D &p, double degrees);
};
}  // namespace Math

#endif  // SRC_TRANSFORMATION_ROTATION_ROTATION_HPP_
