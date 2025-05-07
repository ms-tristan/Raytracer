// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mobius Strip Utils
*/

#ifndef SRC_PRIMITIVE_MOBIUSSTRIP_UTILS_MOBIUSSTRIPUTILS_HPP_
    #define SRC_PRIMITIVE_MOBIUSSTRIP_UTILS_MOBIUSSTRIPUTILS_HPP_
    #include <cmath>
    #include <optional>
    #include "Math/Point3D/Point3D.hpp"
    #include "Math/Vector3D/Vector3D.hpp"
    #include "Ray/Ray.hpp"
    #include "Primitive/IPrimitive.hpp"

namespace RayTracer {
namespace MobiusStripUtils {

struct MobiusPoint {
    Math::Point3D point;
    Math::Vector3D normal;
};

struct MobiusIntersection {
    double distance;
    Math::Point3D hitPoint;
    Math::Vector3D normal;
    bool found;
};

Math::Vector3D calculateNormal(double theta, double s, double majorRadius, double minorRadius);
Math::Point3D calculatePoint(const Math::Point3D& center, double theta, double s, double majorRadius);
MobiusIntersection findClosestIntersection(const Ray& ray, const Math::Point3D& center,
                                          double majorRadius, double minorRadius,
                                          double thickness, double tMin, double tMax);
bool checkBoundingSphereIntersection(const Ray& ray, const Math::Point3D& center,
                                    double boundingSphereRadius);
Ray transformRayForRotation(const Ray& ray, double rotationX, double rotationY, double rotationZ);
Math::Vector3D rotateNormal(const Math::Vector3D& normal, double rotationX, double rotationY, double rotationZ);

}  // namespace MobiusStripUtils
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_MOBIUSSTRIP_UTILS_MOBIUSSTRIPUTILS_HPP_
