// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AmbientLight
*/

#ifndef SRC_LIGHT_AMBIENTLIGHT_AMBIENTLIGHT_HPP_
#define SRC_LIGHT_AMBIENTLIGHT_AMBIENTLIGHT_HPP_
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {
class AmbientLight {
 public:
    Math::Vector3D color;

    AmbientLight();
    explicit AmbientLight(const Math::Vector3D &color);
};
}  // namespace RayTracer

#endif  // SRC_LIGHT_AMBIENTLIGHT_AMBIENTLIGHT_HPP_
