// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mobius Strip Utilities implementation
*/

#include "Primitive/MobiusStrip/Utils/MobiusStripUtils.hpp"
#include "Transformation/Rotate/Rotate.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace RayTracer {
namespace MobiusStripUtils {

Math::Vector3D calculateNormal(double theta, double s, double majorRadius, double minorRadius) {
    // Normal vector on a Möbius strip is perpendicular to the surface at a given point
    double halfTwist = theta / 2.0;
    
    // Normal component due to the twist of the Möbius strip
    Math::Vector3D normal(Math::Coords{
        -std::cos(theta) * std::sin(halfTwist) + s * std::sin(theta) * std::cos(halfTwist),
        -std::sin(theta) * std::sin(halfTwist) - s * std::cos(theta) * std::cos(halfTwist),
        std::cos(halfTwist)
    });
    
    return normal.normalize();
}

Math::Point3D calculatePoint(const Math::Point3D& center, double theta, double s, double majorRadius) {
    // Parametrization of the Möbius strip
    // theta is the angle around the major circle (0 to 2*PI)
    // s is the position along the strip width (-minorRadius to minorRadius)
    double halfTwist = theta / 2.0;
    
    Math::Point3D point(Math::Coords{
        center.X + (majorRadius + s * std::cos(halfTwist)) * std::cos(theta),
        center.Y + (majorRadius + s * std::cos(halfTwist)) * std::sin(theta),
        center.Z + s * std::sin(halfTwist)
    });
    
    return point;
}

// Check if the ray intersects the bounding sphere as a quick rejection test
bool checkBoundingSphereIntersection(const Ray& ray, const Math::Point3D& center, 
                                    double boundingSphereRadius) {
    Math::Vector3D oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - boundingSphereRadius * boundingSphereRadius;
    double discriminant = b * b - 4 * a * c;
    
    return discriminant > 0;
}

// Apply rotation transformations to a ray in reverse order
Ray transformRayForRotation(const Ray& ray, double rotationX, double rotationY, double rotationZ) {
    Ray transformedRay = ray;
    
    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        Math::Point3D newOrigin = ray.origin;
        Math::Vector3D newDirection = ray.direction;
        
        // Apply rotations in reverse order
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

// Apply rotation transformations to a normal vector
Math::Vector3D rotateNormal(const Math::Vector3D& normal, double rotationX, double rotationY, double rotationZ) {
    Math::Vector3D rotatedNormal = normal;
    
    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        // Apply rotations in forward order (opposite to ray transformation)
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

// Find intersection of a ray with a Möbius strip using numerical iteration
MobiusIntersection findClosestIntersection(const Ray& ray, const Math::Point3D& center,
                                          double majorRadius, double minorRadius,
                                          double thickness, double tMin, double tMax) {
    const int numSamples = 100;  // Number of samples along the strip
    const int numIterations = 5; // Number of refinement iterations
    
    MobiusIntersection result;
    result.found = false;
    result.distance = tMax + 1.0; // Initialize with a value beyond tMax

    // Numerical approach: sample points on the Möbius strip and check for intersections
    for (int i = 0; i < numSamples; i++) {
        double theta = (2.0 * M_PI * i) / numSamples;
        
        for (int j = -numSamples/2; j <= numSamples/2; j++) {
            double s = minorRadius * (2.0 * j / numSamples);
            
            // Calculate point on the Möbius strip
            Math::Point3D point = calculatePoint(center, theta, s, majorRadius);
            
            // Calculate vector from ray origin to the point
            Math::Vector3D toPoint = point - ray.origin;
            
            // Project this vector onto the ray direction to get distance along the ray
            double projDistance = toPoint.dot(ray.direction);
            
            // Only consider points in front of the ray and within our t range
            if (projDistance < tMin || projDistance > tMax) 
                continue;
                
            // Calculate the closest point on the ray to our sample point
            Math::Point3D closestPointOnRay = ray.origin + ray.direction * projDistance;
            
            // Calculate distance from the sample point to the ray
            double distance = (point - closestPointOnRay).length();
            
            // If this distance is within the thickness of the strip, we have a hit
            if (distance <= thickness/2.0 && projDistance < result.distance) {
                // Refine the intersection with a few iterations of binary search
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
                    
                    // Update search bounds based on error
                    if (refinedError < distance) {
                        distance = refinedError;
                        projDistance = refinedDistance;
                        point = refinedPoint;
                        theta = thetaMid;
                        s = sMid;
                    }
                    
                    // Narrow down the search range
                    thetaLow = thetaMid - (thetaHigh - thetaLow) / 4.0;
                    thetaHigh = thetaMid + (thetaHigh - thetaLow) / 4.0;
                    sLow = sMid - (sHigh - sLow) / 4.0;
                    sHigh = sMid + (sHigh - sLow) / 4.0;
                }
                
                // We've found a better intersection
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
