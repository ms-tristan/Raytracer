// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone implementation
*/
#include <cmath>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "Primitive/Cone/Cone.hpp"


namespace RayTracer {
Cone::Cone(const Math::Point3D &apex, const Math::Vector3D &axis,
double radius, double height)
: material(std::make_shared<Material>()), apex(apex),
axis(axis.normalize()), radius(radius), height(height) {}

Cone::Cone(const Math::Point3D &apex, const Math::Vector3D &axis,
double radius, double height, const std::shared_ptr<Material> &material)
: material(material), apex(apex), axis(axis.normalize()),
radius(radius), height(height) {}

void Cone::translate(const Math::Vector3D &translation) {
    apex += translation;
}

void Cone::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    apex = rotateX.applyToPoint(apex);
    axis = rotateX.applyToVector(axis).normalize();
}

void Cone::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    apex = rotateY.applyToPoint(apex);
    axis = rotateY.applyToVector(axis).normalize();
}

void Cone::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    apex = rotateZ.applyToPoint(apex);
    axis = rotateZ.applyToVector(axis).normalize();
}

std::shared_ptr<Material> Cone::getMaterial() const {
    return material;
}

std::optional<HitInfo> Cone::hit(const Ray &ray, double tMin,
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

    Math::Vector3D co = transformedRay.origin - apex;

    double cosAngle = height / std::sqrt(height * height + radius * radius);
    double cosAngle2 = cosAngle * cosAngle;

    double oc_dot_axis = co.dot(axis);
    double dir_dot_axis = transformedRay.direction.dot(axis);

    double a = dir_dot_axis * dir_dot_axis -
        cosAngle2 * transformedRay.direction.dot(transformedRay.direction);
    double b = 2.0 * (dir_dot_axis * oc_dot_axis - cosAngle2 *
        co.dot(transformedRay.direction));
    double c = oc_dot_axis * oc_dot_axis - cosAngle2 * co.dot(co);

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

    Math::Point3D hitPoint = transformedRay.origin + transformedRay.direction * t;
    double heightIntersect = (hitPoint - apex).dot(axis);
    if (heightIntersect < 0 || heightIntersect > height) {
        t = t2;
        if (t < tMin || t > tMax)
            return std::nullopt;

        hitPoint = transformedRay.origin + transformedRay.direction * t;
        heightIntersect = (hitPoint - apex).dot(axis);
        if (heightIntersect < 0 || heightIntersect > height)
            return std::nullopt;
    }

    Math::Vector3D cp = hitPoint - apex;
    double m = cp.dot(axis) / axis.dot(axis);
    Math::Point3D axisPoint = apex + axis * m;

    Math::Vector3D normal = (hitPoint - axisPoint).normalize();

    normal = normal + axis * (cosAngle / (1.0 - cosAngle));
    normal = normal.normalize();

    if (normal.dot(transformedRay.direction) > 0)
        normal = normal * -1.0;

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
    Math::Vector3D apexToHit = hitPoint - apex;
    double v = heightIntersect / height;
    Math::Vector3D reference;
    if (std::abs(axis.X) < std::abs(axis.Y) && std::abs(axis.X) < std::abs(axis.Z)) {
        reference = Math::Vector3D(Math::Coords{1, 0, 0}).cross(axis).normalize();
    } else {
        reference = Math::Vector3D(Math::Coords{0, 1, 0}).cross(axis).normalize();
    }
    Math::Vector3D tangent = axis.cross(reference).normalize();
    Math::Vector3D projection = apexToHit - axis * (apexToHit.dot(axis) / axis.dot(axis));
    projection = projection.normalize();
    double cosTheta = projection.dot(reference);
    double sinTheta = projection.dot(tangent);
    double theta = std::atan2(sinTheta, cosTheta);
    double u = (theta + M_PI) / (2.0 * M_PI);
    u = std::fmod(u, 1.0);
    if (u < 0) u += 1.0;
    HitInfo info;
    info.uv = Math::Vector2D(u, v);
    info.distance = t;
    info.hitPoint = ray.origin + ray.direction * t;
    info.normal = normal;
    try {
        info.primitive = std::make_shared<Cone>(*this);
    } catch (const std::exception& e) {
        std::cerr << "Error in Cone::hit(): " << e.what() << std::endl;
        return std::nullopt;
    }
    return info;
}

std::shared_ptr<IPrimitive> Cone::clone() const {
    auto copy = std::make_shared<Cone>(apex, axis, radius, height, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    copy->setSourceFile(sourceFile);
    return copy;
}

void Cone::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& pos = setting->add("apex", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = apex.X;
    pos.add("y", libconfig::Setting::TypeFloat) = apex.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = apex.Z;

    libconfig::Setting& dir = setting->add("direction", libconfig::Setting::TypeGroup);
    dir.add("x", libconfig::Setting::TypeFloat) = axis.X;
    dir.add("y", libconfig::Setting::TypeFloat) = axis.Y;
    dir.add("z", libconfig::Setting::TypeFloat) = axis.Z;

    setting->add("radius", libconfig::Setting::TypeFloat) = radius;
    setting->add("height", libconfig::Setting::TypeFloat) = height;

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
