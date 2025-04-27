// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ILight
*/

#ifndef SRC_LIGHT_ILIGHT_HPP_
#define SRC_LIGHT_ILIGHT_HPP_
#include <memory>
#include <libconfig.h++>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {
class ILight {
 public:
    virtual ~ILight() = default;

    virtual Math::Vector3D getLightDirection(
      const Math::Point3D &point) const = 0;
    virtual Math::Vector3D getLightColor() const = 0;
    virtual std::shared_ptr<ILight> clone() const = 0;
    virtual void getLibConfigParams(libconfig::Setting& setting) const = 0;
};
}  // namespace RayTracer

#endif  // SRC_LIGHT_ILIGHT_HPP_
