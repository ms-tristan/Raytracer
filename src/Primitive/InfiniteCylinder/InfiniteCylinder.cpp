/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InfiniteCylinder implementation
*/

#include "Primitive/InfiniteCylinder/InfiniteCylinder.hpp"
#include <cmath>
#include <memory>
#include <iostream>
#include <vector>
#include <string>

namespace RayTracer {
InfiniteCylinder::InfiniteCylinder(const Math::Point3D &center, const Math::Vector3D &axis,
double radius)
: material(std::make_shared<Material>()), center(center),
axis(axis.normalize()), radius(radius) {}

InfiniteCylinder::InfiniteCylinder(const Math::Point3D &center, const Math::Vector3D &axis,
double radius, const std::shared_ptr<Material> &material)
: material(material), center(center), axis(axis.normalize()),
radius(radius) {}

void InfiniteCylinder::translate(const Math::Vector3D &translation) {
    center += translation;
}

void InfiniteCylinder::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    center = rotateX.applyToPoint(center);
    axis = rotateX.applyToVector(axis).normalize();
}

void InfiniteCylinder::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    center = rotateY.applyToPoint(center);
    axis = rotateY.applyToVector(axis).normalize();
}

void InfiniteCylinder::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    center = rotateZ.applyToPoint(center);
    axis = rotateZ.applyToVector(axis).normalize();
}

std::shared_ptr<Material> InfiniteCylinder::getMaterial() const {
    return material;
}

std::optional<HitInfo> InfiniteCylinder::hit(const Ray &ray,
double tMin, double tMax) {
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
    Math::Point3D axisPoint = center + axis * heightIntersect;
    Math::Vector3D normal = (hitPoint - axisPoint).normalize();
    if (normal.dot(transformedRay.direction) > 0) {
        normal = normal * -1.0;
    }

    Math::Vector3D untransformedNormal = normal;

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

        double dotProduct = normal.dot(axis);
        if (std::abs(dotProduct) > 0.0001) {
            Math::Vector3D parallelComponent = axis * dotProduct;
            normal = (normal - parallelComponent).normalize();
        }
    }

    double v = std::fmod(std::abs(heightIntersect), 1.0);
    Math::Vector3D reference;
    if (std::abs(axis.X) < std::abs(axis.Y) && std::abs(axis.X) < std::abs(axis.Z)) {
        reference = Math::Vector3D(Math::Coords{1, 0, 0}).cross(axis).normalize();
    } else {
        reference = Math::Vector3D(Math::Coords{0, 1, 0}).cross(axis).normalize();
    }
    Math::Vector3D tangent = axis.cross(reference).normalize();
    Math::Vector3D radialVec = hitPoint - axisPoint;
    radialVec = radialVec.normalize();
    double cosTheta = radialVec.dot(reference);
    double sinTheta = radialVec.dot(tangent);
    double theta = std::atan2(sinTheta, cosTheta);
    double u = (theta + M_PI) / (2.0 * M_PI);
    u = std::fmod(u, 1.0);
    if (u < 0) u += 1.0;

    HitInfo info;
    info.distance = t;
    info.hitPoint = ray.origin + ray.direction * t;
    info.normal = normal;
    info.uv = Math::Vector2D(u, v);
    try {
        info.primitive = std::make_shared<InfiniteCylinder>(*this);
    } catch (const std::exception& e) {
        std::cerr << "Error in InfiniteCylinder::hit(): " << e.what() << std::endl;
        return std::nullopt;
    }
    return info;
}

std::shared_ptr<IPrimitive> InfiniteCylinder::clone() const {
    auto copy = std::make_shared<InfiniteCylinder>(center, axis,
        radius, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    copy->setSourceFile(sourceFile);
    return copy;
}

void InfiniteCylinder::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& pos = setting->add("position", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = center.X;
    pos.add("y", libconfig::Setting::TypeFloat) = center.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = center.Z;

    libconfig::Setting& dir = setting->add("direction", libconfig::Setting::TypeGroup);
    dir.add("x", libconfig::Setting::TypeFloat) = axis.X;
    dir.add("y", libconfig::Setting::TypeFloat) = axis.Y;
    dir.add("z", libconfig::Setting::TypeFloat) = axis.Z;

    setting->add("radius", libconfig::Setting::TypeFloat) = radius;

    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        libconfig::Setting& rotation = setting->add("rotation", libconfig::Setting::TypeGroup);
        rotation.add("x", libconfig::Setting::TypeFloat) = rotationX;
        rotation.add("y", libconfig::Setting::TypeFloat) = rotationY;
        rotation.add("z", libconfig::Setting::TypeFloat) = rotationZ;
    }

    libconfig::Setting& mat = setting->add("material", libconfig::Setting::TypeGroup);

    libconfig::Setting& color = mat.add("color", libconfig::Setting::TypeGroup);
    color.add("r", libconfig::Setting::TypeFloat) = material->color.X;
    color.add("g", libconfig::Setting::TypeFloat) = material->color.Y;
    color.add("b", libconfig::Setting::TypeFloat) = material->color.Z;

    mat.add("ambient", libconfig::Setting::TypeFloat) = 0.1;
    mat.add("diffuse", libconfig::Setting::TypeFloat) = 0.9;
    mat.add("reflectivity", libconfig::Setting::TypeFloat) = material->reflectivity;
    mat.add("transparency", libconfig::Setting::TypeFloat) = material->transparency;
    mat.add("refractionIndex", libconfig::Setting::TypeFloat) = material->refractionIndex;
}
}  // namespace RayTracer
