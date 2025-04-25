// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane Primitive
*/

#ifndef SRC_PRIMITIVE_PLANE_PLANE_HPP_
#define SRC_PRIMITIVE_PLANE_PLANE_HPP_
#include <memory>
#include "Primitive/IPrimitive.hpp"
#include "Transformation/Rotate/Rotate.hpp"

namespace RayTracer {
class Plane : public IPrimitive {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;

 public:
    Math::Point3D position;
    Math::Vector3D normal;

    Plane(const Math::Point3D &position, const Math::Vector3D &normal);
    Plane(const Math::Point3D &position, const Math::Vector3D &normal,
          const std::shared_ptr<Material> &material);
    ~Plane() override = default;

    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    std::optional<HitInfo> hit(const Ray &ray,
        double tMin, double tMax) const override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_PLANE_PLANE_HPP_
