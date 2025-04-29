// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** MaterialDecorator
*/
#include <utility>
#include <memory>
#include "MaterialDecorator.hpp"

namespace RayTracer {

MaterialDecorator::MaterialDecorator(std::shared_ptr<IPrimitive> primitive,
std::shared_ptr<Material> material)
: PrimitiveDecorator(std::move(primitive)),
overrideMaterial(std::move(material)) {}

std::shared_ptr<Material> MaterialDecorator::getMaterial() const {
    return overrideMaterial;
}

std::shared_ptr<IPrimitive> MaterialDecorator::clone() const {
    return std::make_shared<MaterialDecorator>(wrappedPrimitive->clone(),
    overrideMaterial);
}

void MaterialDecorator::getLibConfigParams(libconfig::Setting& setting) const {
    PrimitiveDecorator::getLibConfigParams(setting);

    if (setting.exists("material")) {
        setting.remove("material");
    }

    libconfig::Setting& mat = setting.add("material", libconfig::Setting::TypeGroup);

    libconfig::Setting& color = mat.add("color", libconfig::Setting::TypeGroup);
    color.add("r", libconfig::Setting::TypeFloat) = overrideMaterial->color.X;
    color.add("g", libconfig::Setting::TypeFloat) = overrideMaterial->color.Y;
    color.add("b", libconfig::Setting::TypeFloat) = overrideMaterial->color.Z;

    mat.add("ambient", libconfig::Setting::TypeFloat) = 0.1;
    mat.add("diffuse", libconfig::Setting::TypeFloat) = 0.9;
}

}  // namespace RayTracer
