// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane implementation
*/

#include "Primitive/Plane/Plane.hpp"
#include <cmath>
#include <memory>

namespace RayTracer {
Plane::Plane(const Math::Point3D &position, const Math::Vector3D &normal)
    : position(position), normal(normal.normalize()) {}

Plane::Plane(const Math::Point3D &position, const Math::Vector3D &normal,
             const std::shared_ptr<Material> &material)
    : APrimitive(material), position(position), normal(normal.normalize()) {}

void Plane::translate(const Math::Vector3D &translation) { position += translation; }

std::optional<HitInfo> Plane::hit(const Ray &ray, double tMin, double tMax) const {
    double denom = normal.dot(ray.direction);
    if (std::abs(denom) < 1e-8)
        return std::nullopt;

    Math::Vector3D p0l0 = position - ray.origin;
    double t = p0l0.dot(normal) / denom;

    if (t < tMin || t > tMax)
        return std::nullopt;

    HitInfo info;
    info.distance = t;
    info.hitPoint = ray.origin + ray.direction * t;
    info.normal = denom < 0 ? normal : normal * -1.0;
    info.primitive = this;
    return info;
}

std::shared_ptr<IPrimitive> Plane::clone() const {
    return std::make_shared<Plane>(position, normal, material);
}
} // namespace RayTracer
