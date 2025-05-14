/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mobius Strip Utilities implementation
*/

#include <cmath>
#include <algorithm>
#include <iostream>
#include "Primitive/MobiusStrip/Utils/MobiusStripUtils.hpp"
#include "Transformation/Rotate/Rotate.hpp"

namespace RayTracer {
namespace MobiusStripUtils {

Math::Vector3D calculateNormal(double theta, double s, double majorRadius, double minorRadius) {
    double halfTwist = theta / 2.0;
    Math::Vector3D normal(Math::Coords{
        -std::cos(theta) * std::sin(halfTwist) + s * std::sin(theta) * std::cos(halfTwist),
        -std::sin(theta) * std::sin(halfTwist) - s * std::cos(theta) * std::cos(halfTwist),
        std::cos(halfTwist)
    });

    return normal.normalize();
}

Math::Point3D calculatePoint(const Math::Point3D& center, double theta, double s, double majorRadius) {
    double halfTwist = theta / 2.0;

    Math::Point3D point(Math::Coords{
        center.X + (majorRadius + s * std::cos(halfTwist)) * std::cos(theta),
        center.Y + (majorRadius + s * std::cos(halfTwist)) * std::sin(theta),
        center.Z + s * std::sin(halfTwist)
    });

    return point;
}

bool checkBoundingSphereIntersection(const Ray& ray, const Math::Point3D& center, double boundingSphereRadius) {
    Math::Vector3D oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - boundingSphereRadius * boundingSphereRadius;
    double discriminant = b * b - 4 * a * c;

    return discriminant > 0;
}

Ray transformRayForRotation(const Ray& ray, double rotationX, double rotationY, double rotationZ) {
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

    return transformedRay;
}

Math::Vector3D rotateNormal(const Math::Vector3D& normal, double rotationX, double rotationY, double rotationZ) {
    Math::Vector3D rotatedNormal = normal;

    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
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
    }

    return rotatedNormal.normalize();
}

MobiusIntersection findClosestIntersection(const Ray& ray, const Math::Point3D& center,
                                          double majorRadius, double minorRadius,
                                          double thickness, double tMin, double tMax) {
    const int numSamples = 100;
    const int numIterations = 5;

    MobiusIntersection result;
    result.found = false;
    result.distance = tMax + 1.0;

    for (int i = 0; i < numSamples; i++) {
        double theta = (2.0 * M_PI * i) / numSamples;
        for (int j = -numSamples/2; j <= numSamples/2; j++) {
            double s = minorRadius * (2.0 * j / numSamples);
            Math::Point3D point = calculatePoint(center, theta, s, majorRadius);
            Math::Vector3D toPoint = point - ray.origin;
            double projDistance = toPoint.dot(ray.direction);
            if (projDistance < tMin || projDistance > tMax)
                continue;
            Math::Point3D closestPointOnRay = ray.origin + ray.direction * projDistance;
            double distance = (point - closestPointOnRay).length();
            if (distance <= thickness/2.0 && projDistance < result.distance) {
                double thetaLow = theta - (2.0 * M_PI / numSamples);
                double thetaHigh = theta + (2.0 * M_PI / numSamples);
                double sLow = s - (minorRadius / (numSamples/2));
                double sHigh = s + (minorRadius / (numSamples/2));
                for (int iter = 0; iter < numIterations; iter++) {
                    double thetaMid = (thetaLow + thetaHigh) / 2.0;
                    double sMid = (sLow + sHigh) / 2.0;
                    Math::Point3D refinedPoint = calculatePoint(center, thetaMid, sMid, majorRadius);
                    Math::Vector3D toRefinedPoint = refinedPoint - ray.origin;
                    double refinedDistance = toRefinedPoint.dot(ray.direction);
                    Math::Point3D closestToRefined = ray.origin + ray.direction * refinedDistance;
                    double refinedError = (refinedPoint - closestToRefined).length();
                    if (refinedError < distance) {
                        distance = refinedError;
                        projDistance = refinedDistance;
                        point = refinedPoint;
                        theta = thetaMid;
                        s = sMid;
                    }
                    thetaLow = thetaMid - (thetaHigh - thetaLow) / 4.0;
                    thetaHigh = thetaMid + (thetaHigh - thetaLow) / 4.0;
                    sLow = sMid - (sHigh - sLow) / 4.0;
                    sHigh = sMid + (sHigh - sLow) / 4.0;
                }
                result.found = true;
                result.distance = projDistance;
                result.hitPoint = ray.origin + ray.direction * projDistance;
                result.normal = calculateNormal(theta, s, majorRadius, minorRadius);
            }
        }
    }
    return result;
}

}  // namespace MobiusStripUtils
}  // namespace RayTracer
