// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray
*/
#include "Ray/Ray.hpp"
namespace RayTracer {

Ray::Ray(const Math::Point3D &origin, const Math::Vector3D &direction)
: origin(origin), direction(direction) {}

Math::Point3D Ray::at(double t) const {
    return origin + direction * t;
}

}  // namespace RayTracer
