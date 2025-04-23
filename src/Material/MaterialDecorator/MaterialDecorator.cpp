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

}  // namespace RayTracer
