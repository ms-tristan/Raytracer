// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** HitInfo
*/

#ifndef SRC_HITINFO_HPP_
#define SRC_HITINFO_HPP_
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class IPrimitive;

struct HitInfo {
    double distance;
    Math::Point3D hitPoint;
    Math::Vector3D normal;
    const IPrimitive *primitive;
};
}  // namespace RayTracer

#endif  // SRC_HITINFO_HPP_
