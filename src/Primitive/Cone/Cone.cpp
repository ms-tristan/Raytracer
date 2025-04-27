// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone implementation
*/
#include "Primitive/Cone/Cone.hpp"
#include <cmath>
#include <memory>

namespace RayTracer {
Cone::Cone(const Math::Point3D &apex, const Math::Vector3D &axis, double radius, double height)
    : apex(apex), axis(axis.normalize()), radius(radius), height(height) {}

Cone::Cone(const Math::Point3D &apex, const Math::Vector3D &axis, double radius, double height,
           const std::shared_ptr<Material> &material)
    : APrimitive(material), apex(apex), axis(axis.normalize()), radius(radius), height(height) {}

void Cone::translate(const Math::Vector3D &translation) { apex += translation; }

std::optional<HitInfo> Cone::hit(const Ray &ray, double tMin, double tMax) const {
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

    Math::Vector3D co = transformedRay.origin - apex;

    double cosAngle = height / std::sqrt(height * height + radius * radius);
    double cosAngle2 = cosAngle * cosAngle;

    double oc_dot_axis = co.dot(axis);
    double dir_dot_axis = transformedRay.direction.dot(axis);

    double a = dir_dot_axis * dir_dot_axis -
               cosAngle2 * transformedRay.direction.dot(transformedRay.direction);
    double b = 2.0 * (dir_dot_axis * oc_dot_axis - cosAngle2 * co.dot(transformedRay.direction));
    double c = oc_dot_axis * oc_dot_axis - cosAngle2 * co.dot(co);

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

    double heightIntersect = (hitPoint - apex).dot(axis);
    if (heightIntersect < 0 || heightIntersect > height) {
        t = t2;
        if (t < tMin || t > tMax)
            return std::nullopt;

        hitPoint = transformedRay.origin + transformedRay.direction * t;
        heightIntersect = (hitPoint - apex).dot(axis);
        if (heightIntersect < 0 || heightIntersect > height)
            return std::nullopt;
    }

    Math::Vector3D cp = hitPoint - apex;
    double m = cp.dot(axis) / axis.dot(axis);
    Math::Point3D axisPoint = apex + axis * m;

    // Correct normal calculation for a cone
    Math::Vector3D normal = (hitPoint - axisPoint).normalize();
    // Adjust normal direction based on cone angle
    normal = normal + axis * (cosAngle / (1.0 - cosAngle));
    normal = normal.normalize();

    if (normal.dot(transformedRay.direction) > 0)
        normal = normal * -1.0;

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

std::shared_ptr<IPrimitive> Cone::clone() const {
    return std::make_shared<Cone>(apex, axis, radius, height, material);
}

void Cone::getLibConfigParams(libconfig::Setting& setting) const {

    libconfig::Setting& pos = setting.add("apex", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = apex.X;
    pos.add("y", libconfig::Setting::TypeFloat) = apex.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = apex.Z;

    libconfig::Setting& dir = setting.add("direction", libconfig::Setting::TypeGroup);
    dir.add("x", libconfig::Setting::TypeFloat) = axis.X;
    dir.add("y", libconfig::Setting::TypeFloat) = axis.Y;
    dir.add("z", libconfig::Setting::TypeFloat) = axis.Z;

    setting.add("radius", libconfig::Setting::TypeFloat) = radius;
    setting.add("height", libconfig::Setting::TypeFloat) = height;

    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        libconfig::Setting& rotation = setting.add("rotation", libconfig::Setting::TypeGroup);
        rotation.add("x", libconfig::Setting::TypeFloat) = rotationX;
        rotation.add("y", libconfig::Setting::TypeFloat) = rotationY;
        rotation.add("z", libconfig::Setting::TypeFloat) = rotationZ;
    }

    libconfig::Setting& mat = setting.add("material", libconfig::Setting::TypeGroup);

    libconfig::Setting& color = mat.add("color", libconfig::Setting::TypeGroup);
    color.add("r", libconfig::Setting::TypeFloat) = material->color.X;
    color.add("g", libconfig::Setting::TypeFloat) = material->color.Y;
    color.add("b", libconfig::Setting::TypeFloat) = material->color.Z;

    mat.add("ambient", libconfig::Setting::TypeFloat) = 0.1;
    mat.add("diffuse", libconfig::Setting::TypeFloat) = 0.9;
}
}  // namespace RayTracer
