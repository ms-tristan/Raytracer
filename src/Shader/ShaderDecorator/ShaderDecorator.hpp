// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ShaderDecorator - Base decorator for shader composition
*/

#ifndef SRC_SHADER_SHADERDECORATOR_SHADERDECORATOR_HPP_
   #define SRC_SHADER_SHADERDECORATOR_SHADERDECORATOR_HPP_
   #include <memory>
   #include <string>
   #include "../IShader.hpp"

namespace RayTracer {

class ShaderDecorator : public IShader {
 protected:
    std::shared_ptr<IShader> wrappedShader;

 public:
    explicit ShaderDecorator(std::shared_ptr<IShader> shader);
    ~ShaderDecorator() override = default;

    Math::Vector3D apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const override;
    std::string getType() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_SHADERDECORATOR_SHADERDECORATOR_HPP_
