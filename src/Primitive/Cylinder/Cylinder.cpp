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
Cylinder::Cylinder(const Math::Point3D &center, const Math::Vector3D &axis,
double radius, double height)
: material(std::make_shared<Material>()), center(center),
axis(axis.normalize()), radius(radius), height(height) {}

Cylinder::Cylinder(const Math::Point3D &center, const Math::Vector3D &axis,
double radius, double height, const std::shared_ptr<Material> &material)
: material(material), center(center), axis(axis.normalize()),
radius(radius), height(height) {}

void Cylinder::translate(const Math::Vector3D &translation) {
    center += translation;
}

void Cylinder::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    center = rotateX.applyToPoint(center);
    axis = rotateX.applyToVector(axis).normalize();
}

void Cylinder::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    center = rotateY.applyToPoint(center);
    axis = rotateY.applyToVector(axis).normalize();
}

void Cylinder::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    center = rotateZ.applyToPoint(center);
    axis = rotateZ.applyToVector(axis).normalize();
}

std::shared_ptr<Material> Cylinder::getMaterial() const {
    return material;
}

std::optional<HitInfo> Cylinder::hit(const Ray &ray,
double tMin, double tMax) const {
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

    Math::Point3D hitPoint = transformedRay.origin +
        transformedRay.direction * t;

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

    HitInfo info;
    info.distance = t;
    info.hitPoint = ray.origin + ray.direction * t;
    info.normal = normal;
    info.primitive = this;
    return info;
}

std::shared_ptr<IPrimitive> Cylinder::clone() const {
    auto copy = std::make_shared<Cylinder>(center, axis,
        radius, height, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    return copy;
}


void Cylinder::getLibConfigParams(libconfig::Setting& setting) const {

    libconfig::Setting& pos = setting.add("position", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = center.X;
    pos.add("y", libconfig::Setting::TypeFloat) = center.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = center.Z;

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
} // namespace RayTracer
