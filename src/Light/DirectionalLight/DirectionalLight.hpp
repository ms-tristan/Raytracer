// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** DirectionalLight
*/

#ifndef SRC_LIGHT_DIRECTIONALLIGHT_DIRECTIONALLIGHT_HPP_
  #define SRC_LIGHT_DIRECTIONALLIGHT_DIRECTIONALLIGHT_HPP_
  #include <memory>
  #include "Light/ALight/ALight.hpp"

namespace RayTracer {
class DirectionalLight : public ALight {
 public:
    Math::Vector3D direction;

    DirectionalLight(const Math::Vector3D &direction,
      const Math::Vector3D &color);
    ~DirectionalLight() override = default;

    Math::Vector3D getLightDirection(const Math::Point3D &point) const override;
    std::shared_ptr<ILight> clone() const override;
};
}  // namespace RayTracer

#endif  // SRC_LIGHT_DIRECTIONALLIGHT_DIRECTIONALLIGHT_HPP_
