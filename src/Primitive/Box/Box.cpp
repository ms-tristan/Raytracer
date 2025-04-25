// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Box implementation
*/
#include <memory>
#include <algorithm>
#include <cmath>
#include "Primitive/Box/Box.hpp"

namespace RayTracer {
Box::Box(const Math::Point3D &center, const Math::Vector3D &dimensions)
: material(std::make_shared<Material>()), center(center), dimensions(dimensions) {}

Box::Box(const Math::Point3D &center, const Math::Vector3D &dimensions,
const std::shared_ptr<Material> &material)
: material(material), center(center), dimensions(dimensions) {}

void Box::translate(const Math::Vector3D &translation) {
    center += translation;
}

void Box::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    center = rotateX.applyToPoint(center);
}

void Box::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    center = rotateY.applyToPoint(center);
}

void Box::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    center = rotateZ.applyToPoint(center);
}

std::shared_ptr<Material> Box::getMaterial() const {
    return material;
}

std::optional<HitInfo> Box::hit(const Ray &ray, double tMin, double tMax) const {
    Ray transformedRay = ray;
    
    // Apply inverse rotations to the ray if needed
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
    
    // Calculate box bounds
    Math::Point3D min_bound = center - dimensions;
    Math::Point3D max_bound = center + dimensions;
    
    // Ray-AABB intersection algorithm
    double tx1 = (min_bound.X - transformedRay.origin.X) / transformedRay.direction.X;
    double tx2 = (max_bound.X - transformedRay.origin.X) / transformedRay.direction.X;
    double ty1 = (min_bound.Y - transformedRay.origin.Y) / transformedRay.direction.Y;
    double ty2 = (max_bound.Y - transformedRay.origin.Y) / transformedRay.direction.Y;
    double tz1 = (min_bound.Z - transformedRay.origin.Z) / transformedRay.direction.Z;
    double tz2 = (max_bound.Z - transformedRay.origin.Z) / transformedRay.direction.Z;
    
    double tmin = std::max(std::max(std::min(tx1, tx2), std::min(ty1, ty2)), std::min(tz1, tz2));
    double tmax = std::min(std::min(std::max(tx1, tx2), std::max(ty1, ty2)), std::max(tz1, tz2));
    
    // No intersection if tmax < 0 or tmin > tmax
    if (tmax < 0 || tmin > tmax) {
        return std::nullopt;
    }
    
    // Use tmin for intersection if it's > 0, otherwise use tmax
    double t = (tmin > 0) ? tmin : tmax;
    
    // Check if intersection is within the specified range
    if (t < tMin || t > tMax) {
        return std::nullopt;
    }
    
    // Calculate hit information
    HitInfo info;
    info.distance = t;
    info.hitPoint = ray.at(t);
    
    // Determine normal based on which face was hit
    Math::Point3D localHitPoint = transformedRay.at(t);
    Math::Vector3D normal;
    
    // Find which face was hit by checking which component is closest to the box surface
    double epsilon = 1e-6;
    if (std::abs(localHitPoint.X - min_bound.X) < epsilon) {
        normal = Math::Vector3D(Math::Coords{-1, 0, 0});
    } else if (std::abs(localHitPoint.X - max_bound.X) < epsilon) {
        normal = Math::Vector3D(Math::Coords{1, 0, 0});
    } else if (std::abs(localHitPoint.Y - min_bound.Y) < epsilon) {
        normal = Math::Vector3D(Math::Coords{0, -1, 0});
    } else if (std::abs(localHitPoint.Y - max_bound.Y) < epsilon) {
        normal = Math::Vector3D(Math::Coords{0, 1, 0});
    } else if (std::abs(localHitPoint.Z - min_bound.Z) < epsilon) {
        normal = Math::Vector3D(Math::Coords{0, 0, -1});
    } else {
        normal = Math::Vector3D(Math::Coords{0, 0, 1});
    }
    
    // Apply rotations to the normal if needed
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
    
    info.normal = normal.normalize();
    info.primitive = this;
    return info;
}

std::shared_ptr<IPrimitive> Box::clone() const {
    auto copy = std::make_shared<Box>(center, dimensions, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    return copy;
}
}  // namespace RayTracer