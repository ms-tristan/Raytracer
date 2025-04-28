// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PointLight
*/

#ifndef SRC_LIGHT_POINTLIGHT_POINTLIGHT_HPP_
    #define SRC_LIGHT_POINTLIGHT_POINTLIGHT_HPP_

    #include "Light/ALight/ALight.hpp"

    #include <memory>
    #include <libconfig.h++>

namespace RayTracer {
class PointLight : public ALight {
 public:
    Math::Point3D position;
    double constantAttenuation;
    double linearAttenuation;
    double quadraticAttenuation;

    PointLight(const Math::Point3D &position,
               const Math::Vector3D &color,
               double constant = 1.0,
               double linear = 0.09,
               double quadratic = 0.032);
    ~PointLight() override = default;

    Math::Vector3D getLightDirection(const Math::Point3D &point) const override;
    Math::Vector3D getLightColor(const Math::Point3D &point) const;
    std::shared_ptr<ILight> clone() const override;
    void getLibConfigParams(libconfig::Setting& setting) const override;
};
}  // namespace RayTracer

#endif  // SRC_LIGHT_POINTLIGHT_POINTLIGHT_HPP_
