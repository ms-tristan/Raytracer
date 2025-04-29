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
Sphere::Sphere(const Math::Point3D &center,
double radius) : material(std::make_shared<Material>()),
center(center), radius(radius) {}

Sphere::Sphere(const Math::Point3D &center, double radius,
const std::shared_ptr<Material> &material)
: material(material), center(center), radius(radius) {}

void Sphere::translate(const Math::Vector3D &translation) {
    center += translation;
}

void Sphere::rotateX(double degrees) {
    rotationX += degrees;

    RayTracer::Rotate rotateX("x", degrees);
    center = rotateX.applyToPoint(center);
}

void Sphere::rotateY(double degrees) {
    rotationY += degrees;

    RayTracer::Rotate rotateY("y", degrees);
    center = rotateY.applyToPoint(center);
}

void Sphere::rotateZ(double degrees) {
    rotationZ += degrees;

    RayTracer::Rotate rotateZ("z", degrees);
    center = rotateZ.applyToPoint(center);
}

std::shared_ptr<Material> Sphere::getMaterial() const {
    return material;
}

std::optional<HitInfo> Sphere::hit(const Ray &ray, double tMin,
double tMax) const {
    Ray transformedRay = ray;

    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        Math::Point3D newOrigin = ray.origin;
        Math::Vector3D newDirection = ray.direction;

        if (rotationZ != 0.0) {
            RayTracer::Rotate rotateZ("z", -rotationZ);
            newOrigin = rotateZ.applyToPoint(newOrigin);
            newDirection = rotateZ.applyToVector(newDirection);
        }
        if (rotationY != 0.0) {
            RayTracer::Rotate rotateY("y", -rotationY);
            newOrigin = rotateY.applyToPoint(newOrigin);
            newDirection = rotateY.applyToVector(newDirection);
        }
        if (rotationX != 0.0) {
            RayTracer::Rotate rotateX("x", -rotationX);
            newOrigin = rotateX.applyToPoint(newOrigin);
            newDirection = rotateX.applyToVector(newDirection);
        }

        transformedRay = Ray(newOrigin, newDirection);
    }

    Math::Vector3D oc = transformedRay.origin - center;
    double a = transformedRay.direction.dot(transformedRay.direction);
    double b = 2.0 * oc.dot(transformedRay.direction);
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

    Math::Point3D hitPoint = transformedRay.at(root);
    Math::Vector3D normal = (hitPoint - center) / radius;

    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        if (rotationX != 0.0) {
            RayTracer::Rotate rotateX("x", rotationX);
            normal = rotateX.applyToVector(normal);
        }
        if (rotationY != 0.0) {
            RayTracer::Rotate rotateY("y", rotationY);
            normal = rotateY.applyToVector(normal);
        }
        if (rotationZ != 0.0) {
            RayTracer::Rotate rotateZ("z", rotationZ);
            normal = rotateZ.applyToVector(normal);
        }
    }

    info.normal = normal;
    info.primitive = this;
    return info;
}

std::shared_ptr<IPrimitive> Sphere::clone() const {
    auto copy = std::make_shared<Sphere>(center, radius, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    return copy;
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
}  // namespace RayTracer
