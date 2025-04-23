// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere Primitive
*/

#ifndef SRC_PRIMITIVE_SPHERE_SPHERE_HPP_
#define SRC_PRIMITIVE_SPHERE_SPHERE_HPP_
#include "Primitive/APrimitive/APrimitive.hpp"
#include <memory>

namespace RayTracer {
class Sphere : public APrimitive {
  public:
    Math::Point3D center;
    double radius;

    Sphere(const Math::Point3D &center, double radius);
    Sphere(const Math::Point3D &center, double radius, const std::shared_ptr<Material> &material);
    ~Sphere() override = default;

    void translate(const Math::Vector3D &translation) override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin, double tMax) const override;
    std::shared_ptr<IPrimitive> clone() const override;
};
} // namespace RayTracer

#endif // SRC_PRIMITIVE_SPHERE_SPHERE_HPP_
