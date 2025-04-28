// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ColorLightDecorator
*/

#ifndef SRC_LIGHT_LIGHTDECORATOR_COLORLIGHTDECORATOR_HPP_
    #define SRC_LIGHT_LIGHTDECORATOR_COLORLIGHTDECORATOR_HPP_

    #include "LightDecorator.hpp"

    #include <memory>
    #include <libconfig.h++>

namespace RayTracer {

class ColorLightDecorator : public LightDecorator {
 private:
    Math::Vector3D colorFilter;

 public:
    ColorLightDecorator(std::shared_ptr<ILight> light,
        const Math::Vector3D& filter);
    ~ColorLightDecorator() override = default;

    Math::Vector3D getLightColor() const override;
    std::shared_ptr<ILight> clone() const override;
    void getLibConfigParams(libconfig::Setting& setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_LIGHT_LIGHTDECORATOR_COLORLIGHTDECORATOR_HPP_
