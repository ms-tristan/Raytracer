// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** FresnelDecorator - Adds fresnel effect on top of another shader
*/

#ifndef SRC_SHADER_SHADERDECORATOR_FRESNELDECORATOR_HPP_
    #define SRC_SHADER_SHADERDECORATOR_FRESNELDECORATOR_HPP_
    #include <string>
    #include <memory>
    #include "ShaderDecorator.hpp"

namespace RayTracer {

class FresnelDecorator : public ShaderDecorator {
private:
    double fresnelPower;
    Math::Vector3D rimColor;

public:
    FresnelDecorator(std::shared_ptr<IShader> shader, double fresnelPower = 2.0,
        const Math::Vector3D& rimColor = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}));
    ~FresnelDecorator() override = default;

    Math::Vector3D apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const override;
    std::shared_ptr<IShader> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
    std::string getType() const override;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_SHADERDECORATOR_FRESNELDECORATOR_HPP_
