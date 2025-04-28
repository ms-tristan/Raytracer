// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BasicShader - standard lighting shader
*/

#ifndef SRC_SHADER_BASICSHADER_HPP_
#define SRC_SHADER_BASICSHADER_HPP_

#include "AShader.hpp"

namespace RayTracer {

class BasicShader : public AShader {
 public:
    BasicShader();
    ~BasicShader() override = default;

    Math::Vector3D apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const override;
    std::shared_ptr<IShader> clone() const override;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_BASICSHADER_HPP_