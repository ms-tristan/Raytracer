// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Ray
*/

#ifndef SRC_RAY_RAY_HPP_
#define SRC_RAY_RAY_HPP_
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {
class Ray {
 public:
    Math::Point3D origin;
    Math::Vector3D direction;

    Ray() = default;
    Ray(const Math::Point3D &origin, const Math::Vector3D &direction);
};
}  // namespace RayTracer

#endif  // SRC_RAY_RAY_HPP_
