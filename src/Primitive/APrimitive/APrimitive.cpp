// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** APrimitve
*/
#include <memory>
#include <string>
#include "APrimitive.hpp"

namespace RayTracer {
APrimitive::APrimitive() : material(std::make_shared<Material>()) {}

APrimitive::APrimitive(const std::shared_ptr<Material> &material)
: material(material) {}

std::shared_ptr<Material> APrimitive::getMaterial() const { return material; }

void APrimitive::rotateX(double degrees) { rotationX += degrees; }

void APrimitive::rotateY(double degrees) { rotationY += degrees; }

void APrimitive::rotateZ(double degrees) { rotationZ += degrees; }

void APrimitive::setSourceFile(const std::string& source) { sourceFile = source; }

std::string APrimitive::getSourceFile() const { return sourceFile; }

}  // namespace RayTracer
