// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ALight abstract base class
*/

#ifndef SRC_LIGHT_ALIGHT_ALIGHT_HPP_
    #define SRC_LIGHT_ALIGHT_ALIGHT_HPP_
    #include <memory>
    #include <libconfig.h++>
    #include "Light/ILight.hpp"

namespace RayTracer {
class ALight : public ILight {
 protected:
    Math::Vector3D color;

 public:
    ALight();
    explicit ALight(const Math::Vector3D &color);
    virtual ~ALight() = default;

    Math::Vector3D getLightColor() const override;
    std::shared_ptr<ILight> clone() const override = 0;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override = 0;
};
}  // namespace RayTracer

#endif  // SRC_LIGHT_ALIGHT_ALIGHT_HPP_
