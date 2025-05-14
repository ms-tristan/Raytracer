// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle Primitive implementation
*/

#include <memory>
#include <string>
#include <iostream>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Triangle.hpp"
#include "Transformation/Rotate/Rotate.hpp"

namespace RayTracer {
Triangle::Triangle(const Math::Point3D &v1, const Math::Point3D &v2, const Math::Point3D &v3)
    : material(std::make_shared<Material>()),
    vertex1(v1), vertex2(v2), vertex3(v3) {
    Math::Vector3D edge1 = vertex2 - vertex1;
    Math::Vector3D edge2 = vertex3 - vertex1;
    normal = edge1.cross(edge2).normalize();
}

Triangle::Triangle(const Math::Point3D &v1, const Math::Point3D &v2, const Math::Point3D &v3,
        const std::shared_ptr<Material> &material)
    : material(material),
    vertex1(v1), vertex2(v2), vertex3(v3) {
    Math::Vector3D edge1 = vertex2 - vertex1;
    Math::Vector3D edge2 = vertex3 - vertex1;
    normal = edge1.cross(edge2).normalize();
}

void Triangle::translate(const Math::Vector3D &translation) {
    vertex1 = vertex1 + translation;
    vertex2 = vertex2 + translation;
    vertex3 = vertex3 + translation;
}

void Triangle::rotateX(double degrees) {
    rotationX += degrees;
}

void Triangle::rotateY(double degrees) {
    rotationY += degrees;
}

void Triangle::rotateZ(double degrees) {
    rotationZ += degrees;
}

std::optional<HitInfo> Triangle::hit(const Ray &ray, double tMin,
double tMax) {
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

    // Moller-Trumbore algorithm for ray-triangle intersection
    Math::Vector3D edge1 = vertex2 - vertex1;
    Math::Vector3D edge2 = vertex3 - vertex1;
    Math::Vector3D h = transformedRay.direction.cross(edge2);
    double a = edge1.dot(h);

    // Check if ray is parallel to triangle
    if (std::abs(a) < 1e-8)
        return std::nullopt;

    double f = 1.0 / a;
    Math::Vector3D s = transformedRay.origin - vertex1;
    double u = f * s.dot(h);

    // Check if intersection is outside triangle
    if (u < 0.0 || u > 1.0)
        return std::nullopt;

    Math::Vector3D q = s.cross(edge1);
    double v = f * transformedRay.direction.dot(q);

    // Check if intersection is outside triangle
    if (v < 0.0 || u + v > 1.0)
        return std::nullopt;

    // Calculate t
    double t = f * edge2.dot(q);

    // Check if intersection is within the valid range
    if (t < tMin || t > tMax)
        return std::nullopt;

    // Calculate hit information
    Math::Point3D hitPoint = transformedRay.origin + transformedRay.direction * t;
    Math::Vector3D triangleNormal = normal;

    // Make sure normal points against ray direction
    if (triangleNormal.dot(transformedRay.direction) > 0)
        triangleNormal = triangleNormal * -1.0;

    // Apply rotations to the normal if needed
    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        if (rotationX != 0.0) {
            RayTracer::Rotate rotateX("x", rotationX);
            triangleNormal = rotateX.applyToVector(triangleNormal);
        }
        if (rotationY != 0.0) {
            RayTracer::Rotate rotateY("y", rotationY);
            triangleNormal = rotateY.applyToVector(triangleNormal);
        }
        if (rotationZ != 0.0) {
            RayTracer::Rotate rotateZ("z", rotationZ);
            triangleNormal = rotateZ.applyToVector(triangleNormal);
        }
    }

    // Calculate UV coordinates for texture mapping
    double w = 1.0 - u - v;
    double textureU = u;
    double textureV = v;

    HitInfo info;
    info.distance = t;
    info.hitPoint = ray.origin + ray.direction * t;
    info.normal = triangleNormal.normalize();
    info.uv = Math::Vector2D(textureU, textureV);
    info.primitive = shared_from_this();
    return info;
}

std::shared_ptr<Material> Triangle::getMaterial() const {
    return material;
}

std::shared_ptr<IPrimitive> Triangle::clone() const {
    auto copy = std::make_shared<Triangle>(vertex1, vertex2, vertex3, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    copy->setSourceFile(sourceFile);
    return copy;
}

void Triangle::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    (*setting).add("v1x", libconfig::Setting::TypeFloat) = static_cast<double>(vertex1.X);
    (*setting).add("v1y", libconfig::Setting::TypeFloat) = static_cast<double>(vertex1.Y);
    (*setting).add("v1z", libconfig::Setting::TypeFloat) = static_cast<double>(vertex1.Z);
    (*setting).add("v2x", libconfig::Setting::TypeFloat) = static_cast<double>(vertex2.X);
    (*setting).add("v2y", libconfig::Setting::TypeFloat) = static_cast<double>(vertex2.Y);
    (*setting).add("v2z", libconfig::Setting::TypeFloat) = static_cast<double>(vertex2.Z);
    (*setting).add("v3x", libconfig::Setting::TypeFloat) = static_cast<double>(vertex3.X);
    (*setting).add("v3y", libconfig::Setting::TypeFloat) = static_cast<double>(vertex3.Y);
    (*setting).add("v3z", libconfig::Setting::TypeFloat) = static_cast<double>(vertex3.Z);
}

}  // namespace RayTracer
