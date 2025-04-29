// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** FresnelShader
*/

#ifndef SRC_SHADER_FRESNELSHADER_FRESNELSHADER_HPP_
   #define SRC_SHADER_FRESNELSHADER_FRESNELSHADER_HPP_
   #include <memory>
   #include "../AShader.hpp"

namespace RayTracer {

class FresnelShader : public AShader {
 private:
    double specularStrength;
    double shininess;

 public:
    FresnelShader(double fresnelPower, const Math::Vector3D& rimColor);
    ~FresnelShader() override = default;

    Math::Vector3D apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const override;
    std::shared_ptr<IShader> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;

 private:
    double fresnelPower;
    Math::Vector3D rimColor;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_FRESNELSHADER_FRESNELSHADER_HPP_
