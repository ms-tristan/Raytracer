// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IShader interface
*/

#ifndef SRC_SHADER_ISHADER_HPP_
#define SRC_SHADER_ISHADER_HPP_

#include <memory>
#include <libconfig.h++>
#include "../Math/Vector3D/Vector3D.hpp"
#include "../Ray/Ray.hpp"
#include "../defs.hpp"

namespace RayTracer {

class IShader {
 public:
    virtual ~IShader() = default;

    virtual Math::Vector3D apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const = 0;
    virtual std::shared_ptr<IShader> clone() const = 0;
    virtual void getLibConfigParams(libconfig::Setting& setting) const = 0;
    virtual std::string getType() const = 0;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_ISHADER_HPP_