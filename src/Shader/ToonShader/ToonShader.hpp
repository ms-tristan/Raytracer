// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ToonShader - non-photorealistic cel-shading
*/

#ifndef SRC_SHADER_TOONSHADER_TOONSHADER_HPP_
   #define SRC_SHADER_TOONSHADER_TOONSHADER_HPP_
   #include <memory>
   #include "../AShader.hpp"

namespace RayTracer {

class ToonShader : public AShader {
 private:
    int levels;
    double edgeThreshold;

 public:
    ToonShader(int levels = 4, double edgeThreshold = 0.2);
    ~ToonShader() override = default;

    Math::Vector3D apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const override;
    std::shared_ptr<IShader> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_TOONSHADER_TOONSHADER_HPP_
