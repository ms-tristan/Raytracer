// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ShaderDecorator implementation
*/
#include <utility>
#include <string>
#include "ShaderDecorator.hpp"

namespace RayTracer {

ShaderDecorator::ShaderDecorator(std::shared_ptr<IShader> shader)
    : wrappedShader(std::move(shader)) {}

Math::Vector3D ShaderDecorator::apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const {
    return wrappedShader->apply(color, hitInfo, ray);
}

std::string ShaderDecorator::getType() const {
    return "decorator(" + wrappedShader->getType() + ")";
}

void ShaderDecorator::getLibConfigParams(libconfig::Setting& setting) const {
    wrappedShader->getLibConfigParams(setting);

    setting.add("decorated", libconfig::Setting::TypeBoolean) = true;
}

}  // namespace RayTracer
