// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus implementation
*/
#include <cmath>
#include <iostream>
#include <memory>
#include <algorithm>
#include "Primitive/Torus/Torus.hpp"

namespace RayTracer {

Torus::Torus(const Math::Point3D &center, const Math::Vector3D &axis,
double majorRadius, double minorRadius)
: material(std::make_shared<Material>()), center(center),
axis(axis.normalize()), majorRadius(majorRadius), minorRadius(minorRadius) {}

Torus::Torus(const Math::Point3D &center, const Math::Vector3D &axis,
double majorRadius, double minorRadius, const std::shared_ptr<Material> &material)
: material(material), center(center), axis(axis.normalize()),
majorRadius(majorRadius), minorRadius(minorRadius) {}

void Torus::translate(const Math::Vector3D &translation) {
    center += translation;
}

void Torus::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    center = rotateX.applyToPoint(center);
    axis = rotateX.applyToVector(axis).normalize();
}

void Torus::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    center = rotateY.applyToPoint(center);
    axis = rotateY.applyToVector(axis).normalize();
}

void Torus::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    center = rotateZ.applyToPoint(center);
    axis = rotateZ.applyToVector(axis).normalize();
}

std::shared_ptr<Material> Torus::getMaterial() const {
    return material;
}

bool Torus::checkBoundingSphereIntersection(const Ray &ray) const {
    double boundingSphereRadius = majorRadius + minorRadius;
    Math::Vector3D oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - boundingSphereRadius * boundingSphereRadius;
    double discriminant = b * b - 4 * a * c;

    return discriminant >= 0;
}

Ray Torus::transformRayForRotation(const Ray &ray) const {
    if (rotationX == 0.0 && rotationY == 0.0 && rotationZ == 0.0) {
        return ray;
    }
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
    return Ray(newOrigin, newDirection);
}

Math::Vector3D Torus::calculateNormal(const Math::Point3D &hitPoint) const {
    Math::Vector3D oc = hitPoint - center;
    double dotProduct = oc.dot(axis);
    Math::Point3D projectedPoint = hitPoint - axis * dotProduct;

    Math::Vector3D toProjected = projectedPoint - center;
    double projectedDistance = toProjected.length();

    Math::Vector3D toRingCenter = toProjected / projectedDistance;

    Math::Point3D ringCenter = center + toRingCenter * majorRadius;

    return (hitPoint - ringCenter).normalize();
}

std::optional<HitInfo> Torus::hit(const Ray &ray, double tMin, double tMax) {
    if (!checkBoundingSphereIntersection(ray))
        return std::nullopt;

    Ray transformedRay = transformRayForRotation(ray);

    Math::Vector3D oc = transformedRay.origin - center;

    const int MAX_STEPS = 128;
    const double EPSILON = 0.001;
    double t = tMin;

    for (int i = 0; i < MAX_STEPS && t < tMax; i++) {
        Math::Point3D p = transformedRay.origin + transformedRay.direction * t;
        Math::Vector3D op = p - center;

        double dotProduct = op.dot(axis);
        Math::Point3D projectedPoint = p - axis * dotProduct;

        Math::Vector3D toProjected = projectedPoint - center;
        double distToAxis = toProjected.length();

        double distance = std::sqrt(
            std::pow(distToAxis - majorRadius, 2) +
            std::pow(dotProduct, 2)
        ) - minorRadius;

        if (distance < EPSILON) {
            HitInfo info;
            info.distance = t;
            info.hitPoint = ray.origin + ray.direction * t;
            info.normal = calculateNormal(p);

            if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
                if (rotationX != 0.0) {
                    RayTracer::Rotate rotateX("x", rotationX);
                    info.normal = rotateX.applyToVector(info.normal);
                }
                if (rotationY != 0.0) {
                    RayTracer::Rotate rotateY("y", rotationY);
                    info.normal = rotateY.applyToVector(info.normal);
                }
                if (rotationZ != 0.0) {
                    RayTracer::Rotate rotateZ("z", rotationZ);
                    info.normal = rotateZ.applyToVector(info.normal);
                }
            }

            info.normal = info.normal.normalize();
            info.primitive = const_cast<Torus*>(this)->shared_from_this();
            return info;
        }
        t += std::max(distance * 0.5, EPSILON);
    }

    return std::nullopt;
}

std::shared_ptr<IPrimitive> Torus::clone() const {
    auto copy = std::make_shared<Torus>(center, axis, majorRadius, minorRadius, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    return copy;
}

void Torus::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    auto positionSetting = &((*setting).add("position", libconfig::Setting::TypeGroup));
    (*positionSetting).add("x", libconfig::Setting::TypeFloat) = static_cast<double>(center.X);
    (*positionSetting).add("y", libconfig::Setting::TypeFloat) = static_cast<double>(center.Y);
    (*positionSetting).add("z", libconfig::Setting::TypeFloat) = static_cast<double>(center.Z);

    auto axisSetting = &((*setting).add("axis", libconfig::Setting::TypeGroup));
    (*axisSetting).add("x", libconfig::Setting::TypeFloat) = static_cast<double>(axis.X);
    (*axisSetting).add("y", libconfig::Setting::TypeFloat) = static_cast<double>(axis.Y);
    (*axisSetting).add("z", libconfig::Setting::TypeFloat) = static_cast<double>(axis.Z);

    (*setting).add("major_radius", libconfig::Setting::TypeFloat) = static_cast<double>(majorRadius);
    (*setting).add("minor_radius", libconfig::Setting::TypeFloat) = static_cast<double>(minorRadius);

    auto materialSetting = &((*setting).add("material", libconfig::Setting::TypeGroup));
    auto colorSetting = &((*materialSetting).add("color", libconfig::Setting::TypeGroup));
    (*colorSetting).add("r", libconfig::Setting::TypeFloat) = material->color.X;
    (*colorSetting).add("g", libconfig::Setting::TypeFloat) = material->color.Y;
    (*colorSetting).add("b", libconfig::Setting::TypeFloat) = material->color.Z;
    (*materialSetting).add("ambient", libconfig::Setting::TypeFloat) = 0.1;
    (*materialSetting).add("diffuse", libconfig::Setting::TypeFloat) = 0.9;
    (*materialSetting).add("reflectivity", libconfig::Setting::TypeFloat) = material->reflectivity;
    (*materialSetting).add("transparency", libconfig::Setting::TypeFloat) = material->transparency;
    (*materialSetting).add("refractionIndex", libconfig::Setting::TypeFloat) = material->refractionIndex;
}

}  // namespace RayTracer
