// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder implementation
*/

#include "Primitive/Cylinder/Cylinder.hpp"
#include <cmath>
#include <memory>

namespace RayTracer {
Cylinder::Cylinder(const Math::Point3D &center, const Math::Vector3D &axis, double radius,
                   double height)
    : center(center), axis(axis.normalize()), radius(radius), height(height) {}

Cylinder::Cylinder(const Math::Point3D &center, const Math::Vector3D &axis, double radius,
                   double height, const std::shared_ptr<Material> &material)
    : APrimitive(material), center(center), axis(axis.normalize()), radius(radius), height(height) {
}

void Cylinder::translate(const Math::Vector3D &translation) { center += translation; }

std::optional<HitInfo> Cylinder::hit(const Ray &ray, double tMin, double tMax) const {
    Ray transformedRay = ray;
    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        Math::Point3D newOrigin = ray.origin;
        Math::Vector3D newDirection = ray.direction;

        if (rotationZ != 0.0) {
            newOrigin = Math::Rotation::rotateZ(newOrigin, -rotationZ);
            newDirection = Math::Rotation::rotateZ(newDirection, -rotationZ);
        }
        if (rotationY != 0.0) {
            newOrigin = Math::Rotation::rotateY(newOrigin, -rotationY);
            newDirection = Math::Rotation::rotateY(newDirection, -rotationY);
        }
        if (rotationX != 0.0) {
            newOrigin = Math::Rotation::rotateX(newOrigin, -rotationX);
            newDirection = Math::Rotation::rotateX(newDirection, -rotationX);
        }

        transformedRay = Ray(newOrigin, newDirection);
    }

    Math::Vector3D oc = transformedRay.origin - center;

    double a = transformedRay.direction.dot(transformedRay.direction) -
               std::pow(transformedRay.direction.dot(axis), 2);
    double b = 2.0 * (oc.dot(transformedRay.direction) -
                      (oc.dot(axis)) * (transformedRay.direction.dot(axis)));
    double c = oc.dot(oc) - std::pow(oc.dot(axis), 2) - radius * radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return std::nullopt;

    double sqrtd = std::sqrt(discriminant);
    double t1 = (-b - sqrtd) / (2 * a);
    double t2 = (-b + sqrtd) / (2 * a);

    double t = t1;
    if (t < tMin || t > tMax) {
        t = t2;
        if (t < tMin || t > tMax)
            return std::nullopt;
    }

    Math::Point3D hitPoint = transformedRay.origin + transformedRay.direction * t;

    double heightIntersect = (hitPoint - center).dot(axis);
    if (heightIntersect < 0 || heightIntersect > height) {
        t = t2;
        if (t < tMin || t > tMax)
            return std::nullopt;

        hitPoint = transformedRay.origin + transformedRay.direction * t;
        heightIntersect = (hitPoint - center).dot(axis);
        if (heightIntersect < 0 || heightIntersect > height)
            return std::nullopt;
    }

    Math::Point3D axisPoint = center + axis * heightIntersect;
    Math::Vector3D normal = (hitPoint - axisPoint).normalize();

    if (normal.dot(transformedRay.direction) > 0) {
        normal = normal * -1.0;
    }

    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        if (rotationX != 0.0)
            normal = Math::Rotation::rotateX(normal, rotationX);
        if (rotationY != 0.0)
            normal = Math::Rotation::rotateY(normal, rotationY);
        if (rotationZ != 0.0)
            normal = Math::Rotation::rotateZ(normal, rotationZ);
    }

    HitInfo info;
    info.distance = t;
    info.hitPoint = ray.origin + ray.direction * t;
    info.normal = normal;
    info.primitive = this;
    return info;
}

std::shared_ptr<IPrimitive> Cylinder::clone() const {
    return std::make_shared<Cylinder>(center, axis, radius, height, material);
}
} // namespace RayTracer
