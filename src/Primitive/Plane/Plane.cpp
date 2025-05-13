// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane implementation
*/
#include <memory>
#include <cmath>
#include "Primitive/Plane/Plane.hpp"

namespace RayTracer {
Plane::Plane(const Math::Point3D &position, const Math::Vector3D &normal)
: material(std::make_shared<Material>()),
position(position), normal(normal.normalize()) {}

Plane::Plane(const Math::Point3D &position, const Math::Vector3D &normal,
const std::shared_ptr<Material> &material)
: material(material), position(position), normal(normal.normalize()) {}

void Plane::translate(const Math::Vector3D &translation) {
    position += translation;
}

void Plane::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    position = rotateX.applyToPoint(position);
    normal = rotateX.applyToVector(normal).normalize();
}

void Plane::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    position = rotateY.applyToPoint(position);
    normal = rotateY.applyToVector(normal).normalize();
}

void Plane::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    position = rotateZ.applyToPoint(position);
    normal = rotateZ.applyToVector(normal).normalize();
}

std::shared_ptr<Material> Plane::getMaterial() const {
    return material;
}

std::optional<HitInfo> Plane::hit(const Ray &ray, double tMin,
double tMax) {
    Ray transformedRay = ray;
    Math::Vector3D transformedNormal = normal;

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

    double denominator = transformedNormal.dot(transformedRay.direction);

    if (std::abs(denominator) < 1e-8)
        return std::nullopt;

    double t = (position - transformedRay.origin).dot(transformedNormal)
        / denominator;

    if (t < tMin || t > tMax)
        return std::nullopt;

    HitInfo info;
    info.distance = t;
    info.hitPoint = ray.origin + ray.direction * t;
    info.normal = normal;
    if (denominator > 0)
        info.normal = info.normal * -1.0;
    Math::Vector3D tangent1, tangent2;
    if (std::abs(info.normal.X) < std::abs(info.normal.Y) &&
        std::abs(info.normal.X) < std::abs(info.normal.Z)) {
        tangent1 = Math::Vector3D(Math::Coords{1, 0, 0});
    } else if (std::abs(info.normal.Y) < std::abs(info.normal.Z)) {
        tangent1 = Math::Vector3D(Math::Coords{0, 1, 0});
    } else {
        tangent1 = Math::Vector3D(Math::Coords{0, 0, 1});
    }
    tangent1 = tangent1 - info.normal * (info.normal.dot(tangent1));
    tangent1 = tangent1.normalize();
    tangent2 = info.normal.cross(tangent1).normalize();
    Math::Vector3D toPoint = info.hitPoint - position;
    double u = toPoint.dot(tangent1);
    double v = toPoint.dot(tangent2);
    double uvScale = 0.1;
    u = std::fmod(u * uvScale, 1.0);
    v = std::fmod(v * uvScale, 1.0);
    if (u < 0) u += 1.0;
    if (v < 0) v += 1.0;

    info.uv = Math::Vector2D(u, v);
    info.primitive = std::static_pointer_cast<IPrimitive>(shared_from_this());
    return info;
}

std::shared_ptr<IPrimitive> Plane::clone() const {
    auto copy = std::make_shared<Plane>(position, normal, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    copy->setSourceFile(sourceFile);
    return copy;
}

void Plane::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& pos = setting->add("position", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = position.X;
    pos.add("y", libconfig::Setting::TypeFloat) = position.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = position.Z;

    libconfig::Setting& norm = setting->add("normal", libconfig::Setting::TypeGroup);
    norm.add("x", libconfig::Setting::TypeFloat) = normal.X;
    norm.add("y", libconfig::Setting::TypeFloat) = normal.Y;
    norm.add("z", libconfig::Setting::TypeFloat) = normal.Z;

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
