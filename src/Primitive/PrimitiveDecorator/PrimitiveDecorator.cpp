// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrimitiveDecorator
*/
#include <memory>
#include <string>
#include <utility>
#include "PrimitiveDecorator.hpp"

namespace RayTracer {

PrimitiveDecorator::PrimitiveDecorator(std::shared_ptr<IPrimitive> primitive)
: wrappedPrimitive(std::move(primitive)) {}

void PrimitiveDecorator::translate(const Math::Vector3D &translation) {
    wrappedPrimitive->translate(translation);
}

void PrimitiveDecorator::rotateX(double degrees) {
    wrappedPrimitive->rotateX(degrees);
}

void PrimitiveDecorator::rotateY(double degrees) {
    wrappedPrimitive->rotateY(degrees);
}

void PrimitiveDecorator::rotateZ(double degrees) {
    wrappedPrimitive->rotateZ(degrees);
}

void PrimitiveDecorator::setSourceFile(const std::string& source) {
    wrappedPrimitive->setSourceFile(source);
}

std::string PrimitiveDecorator::getSourceFile() const {
    return wrappedPrimitive->getSourceFile();
}

std::optional<HitInfo> PrimitiveDecorator::hit(const Ray &ray,
double tMin, double tMax) {
    return wrappedPrimitive->hit(ray, tMin, tMax);
}

std::shared_ptr<Material> PrimitiveDecorator::getMaterial() const {
    return wrappedPrimitive->getMaterial();
}

void PrimitiveDecorator::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    wrappedPrimitive->getLibConfigParams(setting);
}

}  // namespace RayTracer
