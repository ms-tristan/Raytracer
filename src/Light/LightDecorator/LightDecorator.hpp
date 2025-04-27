// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightDecorator
*/

#ifndef SRC_LIGHT_LIGHTDECORATOR_LIGHTDECORATOR_HPP_
#define SRC_LIGHT_LIGHTDECORATOR_LIGHTDECORATOR_HPP_
#include <memory>
#include <libconfig.h++>
#include "Light/ILight.hpp"

namespace RayTracer {

class LightDecorator : public ILight {
 protected:
    std::shared_ptr<ILight> wrappedLight;

 public:
    explicit LightDecorator(std::shared_ptr<ILight> light);
    ~LightDecorator() override = default;

    Math::Vector3D getLightDirection(const Math::Point3D &point) const override;
    Math::Vector3D getLightColor() const override;
    virtual std::shared_ptr<ILight> clone() const = 0;
    void getLibConfigParams(libconfig::Setting& setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_LIGHT_LIGHTDECORATOR_LIGHTDECORATOR_HPP_
