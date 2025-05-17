// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mobius Strip Utils implementation
*/

#include "Primitive/MobiusStrip/Utils/MobiusStripUtils.hpp"
#include "Transformation/Rotate/Rotate.hpp"

namespace RayTracer {
namespace MobiusStripUtils {

Math::Vector3D calculateNormal(double theta, double s, double majorRadius, double minorRadius) {
    double dx_dtheta = -std::sin(theta) * (majorRadius + s * std::cos(theta / 2.0)) -
                      0.5 * s * std::sin(theta / 2.0) * std::cos(theta);
    double dy_dtheta = std::cos(theta) * (majorRadius + s * std::cos(theta / 2.0)) -
                      0.5 * s * std::sin(theta / 2.0) * std::sin(theta);
    double dz_dtheta = 0.5 * s * std::cos(theta / 2.0);
    double dx_ds = std::cos(theta / 2.0) * std::cos(theta);
    double dy_ds = std::cos(theta / 2.0) * std::sin(theta);
    double dz_ds = std::sin(theta / 2.0);
    Math::Coords tangent1Coords {dx_dtheta, dy_dtheta, dz_dtheta};
    Math::Coords tangent2Coords {dx_ds, dy_ds, dz_ds};
    Math::Vector3D tangent1(tangent1Coords);
    Math::Vector3D tangent2(tangent2Coords);
    return tangent1.cross(tangent2).normalize();
}

Math::Point3D calculatePoint(const Math::Point3D& center, double theta, double s, double majorRadius) {
    double x = (majorRadius + s * std::cos(theta / 2.0)) * std::cos(theta);
    double y = (majorRadius + s * std::cos(theta / 2.0)) * std::sin(theta);
    double z = s * std::sin(theta / 2.0);
    Math::Coords pointCoords {
        center.X + x,
        center.Y + y,
        center.Z + z
    };
    return Math::Point3D(pointCoords);
}

bool checkBoundingSphereIntersection(const Ray& ray, const Math::Point3D& center,
                                    double boundingSphereRadius) {
    Math::Vector3D oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - boundingSphereRadius * boundingSphereRadius;
    double discriminant = b * b - 4 * a * c;

    return discriminant >= 0;
}

Ray transformRayForRotation(const Ray& ray, double rotationX, double rotationY, double rotationZ) {
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

Math::Vector3D rotateNormal(const Math::Vector3D& normal, double rotationX, double rotationY, double rotationZ) {
    if (rotationX == 0.0 && rotationY == 0.0 && rotationZ == 0.0) {
        return normal;
    }
    Math::Vector3D rotatedNormal = normal;
    if (rotationX != 0.0) {
        RayTracer::Rotate rotateX("x", rotationX);
        rotatedNormal = rotateX.applyToVector(rotatedNormal);
    }
    if (rotationY != 0.0) {
        RayTracer::Rotate rotateY("y", rotationY);
        rotatedNormal = rotateY.applyToVector(rotatedNormal);
    }
    if (rotationZ != 0.0) {
        RayTracer::Rotate rotateZ("z", rotationZ);
        rotatedNormal = rotateZ.applyToVector(rotatedNormal);
    }
    return rotatedNormal;
}

MobiusIntersection findClosestIntersection(const Ray& ray, const Math::Point3D& center,
                                          double majorRadius, double minorRadius,
                                          double thickness, double tMin, double tMax) {
    MobiusIntersection result = {tMax, Math::Point3D(), Math::Vector3D(), false};
    const int numSampleTheta = 100;
    const int numSampleWidth = 10;
    for (int i = 0; i < numSampleTheta; i++) {
        double theta = 2.0 * M_PI * i / numSampleTheta;
        for (int j = 0; j < numSampleWidth; j++) {
            double s = minorRadius * (2.0 * j / (numSampleWidth - 1) - 1.0);
            Math::Point3D point = calculatePoint(center, theta, s, majorRadius);
            Math::Vector3D tempNormal = calculateNormal(theta, s, majorRadius, minorRadius);
            Math::Vector3D toPoint = point - ray.origin;
            double proj = toPoint.dot(ray.direction);
            if (proj < tMin || proj > result.distance) {
                continue;
            }
            double distSq = toPoint.dot(toPoint) - proj * proj;
            double thicknessRadiusSq = (thickness / 2.0) * (thickness / 2.0);
            if (distSq <= thicknessRadiusSq) {
                result.distance = proj;
                result.hitPoint = point;
                result.normal = tempNormal;
                result.found = true;
            }
        }
    }
    return result;
}

}  // namespace MobiusStripUtils
}  // namespace RayTracer