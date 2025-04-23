// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone Primitive
*/

#ifndef SRC_PRIMITIVE_CONE_CONE_HPP_
#define SRC_PRIMITIVE_CONE_CONE_HPP_
#include "Primitive/APrimitive/APrimitive.hpp"
#include "Transformation/Rotation/Rotation.hpp"
#include <memory>

namespace RayTracer {
class Cone : public APrimitive {
  public:
    Math::Point3D apex;
    Math::Vector3D axis;
    double radius;
    double height;

    Cone(const Math::Point3D &apex, const Math::Vector3D &axis, double radius, double height);
    Cone(const Math::Point3D &apex, const Math::Vector3D &axis, double radius, double height,
         const std::shared_ptr<Material> &material);
    ~Cone() override = default;

    void translate(const Math::Vector3D &translation) override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin, double tMax) const override;
    std::shared_ptr<IPrimitive> clone() const override;
};
} // namespace RayTracer

#endif // SRC_PRIMITIVE_CONE_CONE_HPP_
