// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere implementation
*/
#include "Primitive/Sphere/Sphere.hpp"
#include <cmath>
#include <memory>

namespace RayTracer {
Sphere::Sphere(const Math::Point3D &center, double radius) : center(center), radius(radius) {}

Sphere::Sphere(const Math::Point3D &center, double radius,
               const std::shared_ptr<Material> &material)
    : APrimitive(material), center(center), radius(radius) {}

void Sphere::translate(const Math::Vector3D &translation) { center += translation; }

std::optional<HitInfo> Sphere::hit(const Ray &ray, double tMin, double tMax) const {
    Math::Vector3D oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return std::nullopt;

    double sqrtd = std::sqrt(discriminant);
    double root = (-b - sqrtd) / (2.0 * a);
    if (root < tMin || tMax < root) {
        root = (-b + sqrtd) / (2.0 * a);
        if (root < tMin || tMax < root)
            return std::nullopt;
    }

    HitInfo info;
    info.distance = root;
    info.hitPoint = ray.origin + ray.direction * root;
    info.normal = (info.hitPoint - center) / radius;
    info.primitive = this;
    return info;
}

std::shared_ptr<IPrimitive> Sphere::clone() const {
    return std::make_shared<Sphere>(center, radius, material);
}

void Sphere::getLibConfigParams(libconfig::Setting& setting) const {

    libconfig::Setting& pos = setting.add("position", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = center.X;
    pos.add("y", libconfig::Setting::TypeFloat) = center.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = center.Z;

    setting.add("radius", libconfig::Setting::TypeFloat) = radius;

    libconfig::Setting& mat = setting.add("material", libconfig::Setting::TypeGroup);

    libconfig::Setting& color = mat.add("color", libconfig::Setting::TypeGroup);
    color.add("r", libconfig::Setting::TypeFloat) = material->color.X;
    color.add("g", libconfig::Setting::TypeFloat) = material->color.Y;
    color.add("b", libconfig::Setting::TypeFloat) = material->color.Z;

    mat.add("ambient", libconfig::Setting::TypeFloat) = 0.1;  // Default ambient
    mat.add("diffuse", libconfig::Setting::TypeFloat) = 0.9;  // Default diffuse
}
} // namespace RayTracer
