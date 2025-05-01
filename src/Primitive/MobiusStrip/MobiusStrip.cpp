// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mobius Strip implementation
*/
#include "Primitive/MobiusStrip/MobiusStrip.hpp"
#include <cmath>
#include <memory>
#include <algorithm>

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

// This method implements ray-Möbius strip intersection
// It uses an iterative approach to find intersections with the parametric surface
std::optional<HitInfo> MobiusStrip::hit(const Ray &ray, double tMin, double tMax) {
    // Transform ray to object space if rotation is applied
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

    // We'll use Newton-Raphson method to find intersections
    // First, we need to check if the ray might intersect with the strip's bounding sphere
    double boundingSphereRadius = majorRadius + minorRadius + thickness/2.0;
    Math::Vector3D oc = transformedRay.origin - center;
    double a = transformedRay.direction.dot(transformedRay.direction);
    double b = 2.0 * oc.dot(transformedRay.direction);
    double c = oc.dot(oc) - boundingSphereRadius * boundingSphereRadius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return std::nullopt; // No intersection with bounding sphere, so no hit
    }

    // Newton-Raphson iteration to find the intersection point
    // This is a simplified approximation - for production, would need more robust algorithm
    double closestT = tMax;
    bool hitFound = false;
    Math::Point3D hitPoint;
    Math::Vector3D normal;
    // Sample a few points on the Möbius strip for potential intersection
    // In a full implementation, this would use proper numerical methods
    // The Möbius strip parametric equation: 
    // x = (R + s*cos(t/2))*cos(t)
    // y = (R + s*cos(t/2))*sin(t)
    // z = s*sin(t/2)
    // where t ∈ [0,2π], s ∈ [-w,w], R is major radius, w is minor radius
    const int numSampleTheta = 100;  // Number of samples around the strip
    const int numSampleWidth = 10;   // Number of samples across the width
    for (int i = 0; i < numSampleTheta; i++) {
        double theta = 2.0 * M_PI * i / numSampleTheta;
        for (int j = 0; j < numSampleWidth; j++) {
            double s = minorRadius * (2.0 * j / (numSampleWidth - 1) - 1.0);
            // Calculate point on Möbius strip
            double x = (majorRadius + s * std::cos(theta / 2.0)) * std::cos(theta);
            double y = (majorRadius + s * std::cos(theta / 2.0)) * std::sin(theta);
            double z = s * std::sin(theta / 2.0);
            // Create a point using Coords struct
            Math::Coords pointCoords {
                center.X + x,
                center.Y + y,
                center.Z + z
            };
            Math::Point3D point(pointCoords);
            // Calculate normal at this point
            // Partial derivatives of the parametric equations give us tangent vectors
            // Cross product of tangent vectors gives the normal
            double dx_dtheta = -std::sin(theta) * (majorRadius + s * std::cos(theta / 2.0)) -
                              0.5 * s * std::sin(theta / 2.0) * std::cos(theta);
            double dy_dtheta = std::cos(theta) * (majorRadius + s * std::cos(theta / 2.0)) -
                              0.5 * s * std::sin(theta / 2.0) * std::sin(theta);
            double dz_dtheta = 0.5 * s * std::cos(theta / 2.0);
            double dx_ds = std::cos(theta / 2.0) * std::cos(theta);
            double dy_ds = std::cos(theta / 2.0) * std::sin(theta);
            double dz_ds = std::sin(theta / 2.0);
            // Create vectors using Coords stru
            Math::Coords tangent1Coords {dx_dtheta, dy_dtheta, dz_dtheta};
            Math::Coords tangent2Coords {dx_ds, dy_ds, dz_ds};
            Math::Vector3D tangent1(tangent1Coords);
            Math::Vector3D tangent2(tangent2Coords);
            Math::Vector3D tempNormal = tangent1.cross(tangent2).normalize();
            // Check for intersection with a small sphere around this point
            // This is a simplification for the ray-surface intersection
            Math::Vector3D toPoint = point - transformedRay.origin;
            double proj = toPoint.dot(transformedRay.direction);
            if (proj < tMin || proj > closestT) continue;
            double distSq = toPoint.dot(toPoint) - proj * proj;
            double thicknessRadiusSq = (thickness / 2.0) * (thickness / 2.0);
            if (distSq <= thicknessRadiusSq) {
                closestT = proj;
                hitPoint = point;
                normal = tempNormal;
                hitFound = true;
            }
        }
    }
    if (!hitFound) return std::nullopt;
    // Apply rotation to normal if needed
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
    // Ensure normal faces against the ray
    if (normal.dot(ray.direction) > 0) {
        normal = normal * -1.0;
    }
    HitInfo info;
    info.distance = closestT;
    info.hitPoint = ray.origin + ray.direction * closestT;
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
