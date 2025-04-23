// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IPrimitive
*/

#ifndef SRC_PRIMITIVE_IPRIMITIVE_HPP_
#define SRC_PRIMITIVE_IPRIMITIVE_HPP_
#include <memory>
#include <optional>
#include "HitInfo.hpp"
#include "Material/Material.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Ray/Ray.hpp"

namespace RayTracer {
class IPrimitive {
 public:
    virtual ~IPrimitive() = default;

    virtual void translate(const Math::Vector3D &translation) = 0;
    virtual void rotateX(double degrees) = 0;
    virtual void rotateY(double degrees) = 0;
    virtual void rotateZ(double degrees) = 0;
    virtual std::optional<HitInfo> hit(const Ray &ray,
      double tMin, double tMax) const = 0;
    virtual std::shared_ptr<Material> getMaterial() const = 0;
    virtual std::shared_ptr<IPrimitive> clone() const = 0;
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_IPRIMITIVE_HPP_
