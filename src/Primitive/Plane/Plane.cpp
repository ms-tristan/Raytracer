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

void Plane::getLibConfigParams(libconfig::Setting& setting) const {

    libconfig::Setting& pos = setting.add("position", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = position.X;
    pos.add("y", libconfig::Setting::TypeFloat) = position.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = position.Z;

    libconfig::Setting& norm = setting.add("normal", libconfig::Setting::TypeGroup);
    norm.add("x", libconfig::Setting::TypeFloat) = normal.X;
    norm.add("y", libconfig::Setting::TypeFloat) = normal.Y;
    norm.add("z", libconfig::Setting::TypeFloat) = normal.Z;

    libconfig::Setting& mat = setting.add("material", libconfig::Setting::TypeGroup);

    libconfig::Setting& color = mat.add("color", libconfig::Setting::TypeGroup);
    color.add("r", libconfig::Setting::TypeFloat) = material->color.X;
    color.add("g", libconfig::Setting::TypeFloat) = material->color.Y;
    color.add("b", libconfig::Setting::TypeFloat) = material->color.Z;

    mat.add("ambient", libconfig::Setting::TypeFloat) = 0.1;
    mat.add("diffuse", libconfig::Setting::TypeFloat) = 0.9;
}
} // namespace RayTracer
