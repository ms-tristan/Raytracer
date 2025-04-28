// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PhongShader - advanced lighting with specular highlights
*/

#ifndef SRC_SHADER_PHONGSHADER_HPP_
#define SRC_SHADER_PHONGSHADER_HPP_

#include "../AShader.hpp"

namespace RayTracer {

class PhongShader : public AShader {
 private:
    double specularStrength;
    double shininess;
 
 public:
    PhongShader(double specularStrength = 0.5, double shininess = 32.0);
    ~PhongShader() override = default;

    Math::Vector3D apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const override;
    std::shared_ptr<IShader> clone() const override;
    void getLibConfigParams(libconfig::Setting& setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_PHONGSHADER_HPP_