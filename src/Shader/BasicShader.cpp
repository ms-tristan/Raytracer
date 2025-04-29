// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BasicShader implementation
*/
#include <memory>
#include "BasicShader.hpp"

namespace RayTracer {

BasicShader::BasicShader() : AShader("basic") {}

Math::Vector3D BasicShader::apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const {
    return color;
}

std::shared_ptr<IShader> BasicShader::clone() const {
    return std::make_shared<BasicShader>();
}

}  // namespace RayTracer
