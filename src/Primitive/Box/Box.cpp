// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Box implementation
*/
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "Primitive/Box/Box.hpp"

namespace RayTracer {
Box::Box(const Math::Point3D &center, const Math::Vector3D &dimensions)
: material(std::make_shared<Material>()), center(center),
dimensions(dimensions) {}

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

std::optional<HitInfo> Box::hit(const Ray &ray,
double tMin, double tMax) {
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

    Math::Point3D min_bound = center - dimensions;
    Math::Point3D max_bound = center + dimensions;

    double tx1 = (min_bound.X - transformedRay.origin.X)
        / transformedRay.direction.X;
    double tx2 = (max_bound.X - transformedRay.origin.X)
        / transformedRay.direction.X;
    double ty1 = (min_bound.Y - transformedRay.origin.Y)
        / transformedRay.direction.Y;
    double ty2 = (max_bound.Y - transformedRay.origin.Y)
        / transformedRay.direction.Y;
    double tz1 = (min_bound.Z - transformedRay.origin.Z)
        / transformedRay.direction.Z;
    double tz2 = (max_bound.Z - transformedRay.origin.Z)
        / transformedRay.direction.Z;

    double tmin = std::max(std::max(std::min(tx1, tx2),
        std::min(ty1, ty2)), std::min(tz1, tz2));
    double tmax = std::min(std::min(std::max(tx1, tx2),
        std::max(ty1, ty2)), std::max(tz1, tz2));

    if (tmax < 0 || tmin > tmax)
        return std::nullopt;

    double t = (tmin > 0) ? tmin : tmax;

    if (t < tMin || t > tMax)
        return std::nullopt;

    HitInfo info;
    info.distance = t;
    info.hitPoint = ray.at(t);
    Math::Point3D localHitPoint = transformedRay.at(t);
    Math::Vector3D normal;

    double epsilon = 1e-6;
    int faceIndex = 0;
    if (std::abs(localHitPoint.X - min_bound.X) < epsilon) {
        normal = Math::Vector3D(Math::Coords{-1, 0, 0});
        faceIndex = 0;
    } else if (std::abs(localHitPoint.X - max_bound.X) < epsilon) {
        normal = Math::Vector3D(Math::Coords{1, 0, 0});
        faceIndex = 1;
    } else if (std::abs(localHitPoint.Y - min_bound.Y) < epsilon) {
        normal = Math::Vector3D(Math::Coords{0, -1, 0});
        faceIndex = 2;
    } else if (std::abs(localHitPoint.Y - max_bound.Y) < epsilon) {
        normal = Math::Vector3D(Math::Coords{0, 1, 0});
        faceIndex = 3;
    } else if (std::abs(localHitPoint.Z - min_bound.Z) < epsilon) {
        normal = Math::Vector3D(Math::Coords{0, 0, -1});
        faceIndex = 4;
    } else {
        normal = Math::Vector3D(Math::Coords{0, 0, 1});
        faceIndex = 5;
    }

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

    double u = 0.0, v = 0.0;
    switch (faceIndex) {
        case 0:
            u = (localHitPoint.Z - min_bound.Z) / (max_bound.Z - min_bound.Z);
            v = (localHitPoint.Y - min_bound.Y) / (max_bound.Y - min_bound.Y);
            break;
        case 1:
            u = 1.0 - (localHitPoint.Z - min_bound.Z) / (max_bound.Z - min_bound.Z);
            v = (localHitPoint.Y - min_bound.Y) / (max_bound.Y - min_bound.Y);
            break;
        case 2:
            u = (localHitPoint.X - min_bound.X) / (max_bound.X - min_bound.X);
            v = (localHitPoint.Z - min_bound.Z) / (max_bound.Z - min_bound.Z);
            break;
        case 3:
            u = (localHitPoint.X - min_bound.X) / (max_bound.X - min_bound.X);
            v = 1.0 - (localHitPoint.Z - min_bound.Z) / (max_bound.Z - min_bound.Z);
            break;
        case 4:
            u = 1.0 - (localHitPoint.X - min_bound.X) / (max_bound.X - min_bound.X);
            v = (localHitPoint.Y - min_bound.Y) / (max_bound.Y - min_bound.Y);
            break;
        case 5:
            u = (localHitPoint.X - min_bound.X) / (max_bound.X - min_bound.X);
            v = (localHitPoint.Y - min_bound.Y) / (max_bound.Y - min_bound.Y);
            break;
    }
    u = std::clamp(u, 0.0, 1.0);
    v = std::clamp(v, 0.0, 1.0);
    info.uv = Math::Vector2D(u, v);
    info.normal = normal.normalize();
    try {
        // Au lieu d'utiliser shared_from_this() qui peut échouer dans un contexte de plugin,
        // assignons directement un nouveau shared_ptr à l'objet courant
        info.primitive = std::make_shared<Box>(*this);
    } catch (const std::exception& e) {
        std::cerr << "Error in Box::hit(): " << e.what() << std::endl;
        return std::nullopt;
    }
    return info;
}

std::shared_ptr<IPrimitive> Box::clone() const {
    auto copy = std::make_shared<Box>(center, dimensions, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    copy->setSourceFile(sourceFile);
    return copy;
}

void Box::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    setting->add("type", libconfig::Setting::TypeString) = "box";

    libconfig::Setting& pos = setting->add("center", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = center.X;
    pos.add("y", libconfig::Setting::TypeFloat) = center.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = center.Z;

    libconfig::Setting& dim = setting->add("dimensions", libconfig::Setting::TypeGroup);
    dim.add("x", libconfig::Setting::TypeFloat) = dimensions.X;
    dim.add("y", libconfig::Setting::TypeFloat) = dimensions.Y;
    dim.add("z", libconfig::Setting::TypeFloat) = dimensions.Z;

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
