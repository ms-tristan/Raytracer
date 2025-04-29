// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CompositePrimitive
*/
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>
#include <typeinfo>
#include "CompositePrimitive.hpp"
#include "Transformation/Rotate/Rotate.hpp"
#include "Primitive/Plugin/PrimitivePluginManager.hpp"

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
double tMax) {
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

void CompositePrimitive::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    // libconfig::Setting& mat = setting->add("material", libconfig::Setting::TypeGroup);

    // libconfig::Setting& color = mat.add("color", libconfig::Setting::TypeGroup);
    // color.add("r", libconfig::Setting::TypeFloat) = material->color.X;
    // color.add("g", libconfig::Setting::TypeFloat) = material->color.Y;
    // color.add("b", libconfig::Setting::TypeFloat) = material->color.Z;

    // mat.add("ambient", libconfig::Setting::TypeFloat) = 0.1;
    // mat.add("diffuse", libconfig::Setting::TypeFloat) = 0.9;

    // libconfig::Setting& children = setting->add("children", libconfig::Setting::TypeList);


    // for (const auto& primitive : primitives) {
    //     libconfig::Setting& child = children.add(libconfig::Setting::TypeGroup);

    //     const std::type_info& type = typeid(*primitive);
    //     std::string primitiveType = "unknown";

    //     // if (type == typeid(Sphere)) primitiveType = "sphere";
    //     // else if (type == typeid(Plane)) primitiveType = "plane";
    //     // else if (type == typeid(Cylinder)) primitiveType = "cylinder";
    //     // else if (type == typeid(Cone)) primitiveType = "cone";

    //     child.add("type", libconfig::Setting::TypeString) = primitiveType;

    //     auto childSettingPtr = std::make_shared<libconfig::Setting>(child);
    //     primitive->getLibConfigParams(childSettingPtr);
    // }
}

}  // namespace RayTracer
