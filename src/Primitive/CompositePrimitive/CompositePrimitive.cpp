// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CompositePrimitive
*/
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>
#include "CompositePrimitive.hpp"
#include "Transformation/Rotate/Rotate.hpp"

namespace RayTracer {

CompositePrimitive::CompositePrimitive(std::shared_ptr<Material> material)
: material(std::move(material)) {}

void CompositePrimitive::translate(const Math::Vector3D &translation) {
    for (auto &primitive : primitives) {
        primitive->translate(translation);
    }
}

void CompositePrimitive::rotateX(double degrees) {
    rotationX += degrees;
    for (auto &primitive : primitives) {
        primitive->rotateX(degrees);
    }
}

void CompositePrimitive::rotateY(double degrees) {
    rotationY += degrees;
    for (auto &primitive : primitives) {
        primitive->rotateY(degrees);
    }
}

void CompositePrimitive::rotateZ(double degrees) {
    rotationZ += degrees;
    for (auto &primitive : primitives) {
        primitive->rotateZ(degrees);
    }
}

std::optional<HitInfo> CompositePrimitive::hit(const Ray &ray, double tMin,
double tMax) const {
    std::optional<HitInfo> closestHit;
    double closest = tMax;

    for (const auto &primitive : primitives) {
        auto hit = primitive->hit(ray, tMin, closest);
        if (hit) {
            closestHit = hit;
            closest = hit->distance;
        }
    }

    return closestHit;
}

std::shared_ptr<Material> CompositePrimitive::getMaterial() const {
    return material;
}

std::shared_ptr<IPrimitive> CompositePrimitive::clone() const {
    auto copy = std::make_shared<CompositePrimitive>(material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;

    for (const auto &primitive : primitives) {
        copy->add(primitive->clone());
    }
    return copy;
}

void CompositePrimitive::add(std::shared_ptr<IPrimitive> primitive) {
    primitives.push_back(std::move(primitive));
}

void CompositePrimitive::remove(std::shared_ptr<IPrimitive> primitive) {
    primitives.erase(
        std::remove(primitives.begin(), primitives.end(), primitive),
        primitives.end());
}

const std::vector<std::shared_ptr<IPrimitive>>&
CompositePrimitive::getPrimitives() const {
    return primitives;
}

}  // namespace RayTracer
