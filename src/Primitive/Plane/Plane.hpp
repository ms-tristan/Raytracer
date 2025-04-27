// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane Primitive
*/

#ifndef SRC_PRIMITIVE_PLANE_PLANE_HPP_
#define SRC_PRIMITIVE_PLANE_PLANE_HPP_
#include "Primitive/APrimitive/APrimitive.hpp"
#include <memory>
#include <libconfig.h++>

namespace RayTracer {
class Plane : public APrimitive {
  public:
    Math::Point3D position;
    Math::Vector3D normal;

    Plane(const Math::Point3D &position, const Math::Vector3D &normal);
    Plane(const Math::Point3D &position, const Math::Vector3D &normal,
          const std::shared_ptr<Material> &material);
    ~Plane() override = default;

    void translate(const Math::Vector3D &translation) override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin, double tMax) const override;
    std::shared_ptr<IPrimitive> clone() const override;
    void getLibConfigParams(libconfig::Setting& setting) const override;
};
} // namespace RayTracer

#endif // SRC_PRIMITIVE_PLANE_PLANE_HPP_
