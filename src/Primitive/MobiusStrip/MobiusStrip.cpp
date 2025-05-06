// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mobius Strip implementation
*/
#include <cmath>
#include <memory>
#include <algorithm>
#include "Primitive/MobiusStrip/MobiusStrip.hpp"
#include "Primitive/MobiusStrip/Utils/MobiusStripUtils.hpp"

namespace RayTracer {
MobiusStrip::MobiusStrip(const Math::Point3D &center, double majorRadius,
double minorRadius, double thickness)
: material(std::make_shared<Material>()), center(center),
majorRadius(majorRadius), minorRadius(minorRadius), thickness(thickness) {}

MobiusStrip::MobiusStrip(const Math::Point3D &center, double majorRadius,
double minorRadius, double thickness, const std::shared_ptr<Material> &material)
: material(material), center(center), majorRadius(majorRadius),
minorRadius(minorRadius), thickness(thickness) {}

void MobiusStrip::translate(const Math::Vector3D &translation) {
    center += translation;
}

void MobiusStrip::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    center = rotateX.applyToPoint(center);
}

void MobiusStrip::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    center = rotateY.applyToPoint(center);
}

void MobiusStrip::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    center = rotateZ.applyToPoint(center);
}

std::shared_ptr<Material> MobiusStrip::getMaterial() const {
    return material;
}

std::optional<HitInfo> MobiusStrip::hit(const Ray &ray, double tMin, double tMax) {
    Ray transformedRay = MobiusStripUtils::transformRayForRotation(ray, rotationX, rotationY, rotationZ);
    double boundingSphereRadius = majorRadius + minorRadius + thickness/2.0;
    if (!MobiusStripUtils::checkBoundingSphereIntersection(transformedRay, center, boundingSphereRadius)) {
        return std::nullopt;
    }
    MobiusStripUtils::MobiusIntersection intersection =
        MobiusStripUtils::findClosestIntersection(transformedRay, center, majorRadius,
                                                 minorRadius, thickness, tMin, tMax);
    if (!intersection.found) {
        return std::nullopt;
    }
    Math::Vector3D normal = MobiusStripUtils::rotateNormal(intersection.normal,
                                                          rotationX, rotationY, rotationZ);
    if (normal.dot(ray.direction) > 0) {
        normal = normal * -1.0;
    }
    HitInfo info;
    info.distance = intersection.distance;
    info.hitPoint = ray.origin + ray.direction * intersection.distance;
    info.normal = normal;
    info.primitive = shared_from_this();
    return info;
}

std::shared_ptr<IPrimitive> MobiusStrip::clone() const {
    auto copy = std::make_shared<MobiusStrip>(center, majorRadius, minorRadius, thickness, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    return copy;
}

void MobiusStrip::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    (*setting)["x"] = center.X;
    (*setting)["y"] = center.Y;
    (*setting)["z"] = center.Z;
    (*setting)["major_radius"] = majorRadius;
    (*setting)["minor_radius"] = minorRadius;
    (*setting)["thickness"] = thickness;
}

}  // namespace RayTracer
