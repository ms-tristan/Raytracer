// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera
*/

#ifndef SRC_CAMERA_CAMERA_HPP_
#define SRC_CAMERA_CAMERA_HPP_
#include "Math/Point3D/Point3D.hpp"
#include "Ray/Ray.hpp"
#include "Rectangle3D/Rectangle3D.hpp"

namespace RayTracer {
class Camera {
  public:
    Math::Point3D origin;
    Rectangle3D screen;

    Camera();
    Camera(const Math::Point3D &origin, const Rectangle3D &screen);
    Ray ray(double u, double v) const;
};
} // namespace RayTracer

#endif // SRC_CAMERA_CAMERA_HPP_
