// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder Primitive
*/

#ifndef SRC_PRIMITIVE_CYLINDER_CYLINDER_HPP_
#define SRC_PRIMITIVE_CYLINDER_CYLINDER_HPP_
#include "Primitive/APrimitive/APrimitive.hpp"
#include "Transformation/Rotation/Rotation.hpp"
#include <memory>

namespace RayTracer {
class Cylinder : public APrimitive {
  public:
    Math::Point3D center;
    Math::Vector3D axis;
    double radius;
    double height;

    Cylinder(const Math::Point3D &center, const Math::Vector3D &axis, double radius, double height);
    Cylinder(const Math::Point3D &center, const Math::Vector3D &axis, double radius, double height,
             const std::shared_ptr<Material> &material);
    ~Cylinder() override = default;

    void translate(const Math::Vector3D &translation) override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin, double tMax) const override;
    std::shared_ptr<IPrimitive> clone() const override;
};
} // namespace RayTracer

#endif // SRC_PRIMITIVE_CYLINDER_CYLINDER_HPP_
