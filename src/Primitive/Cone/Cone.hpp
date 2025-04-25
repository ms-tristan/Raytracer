// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone Primitive
*/

#ifndef SRC_PRIMITIVE_CONE_CONE_HPP_
#define SRC_PRIMITIVE_CONE_CONE_HPP_
#include <memory>
#include "Primitive/IPrimitive.hpp"
#include "Transformation/Rotate/Rotate.hpp"

namespace RayTracer {
class Cone : public IPrimitive {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;

 public:
    Math::Point3D apex;
    Math::Vector3D axis;
    double radius;
    double height;

    Cone(const Math::Point3D &apex, const Math::Vector3D &axis,
        double radius, double height);
    Cone(const Math::Point3D &apex, const Math::Vector3D &axis,
        double radius, double height,
        const std::shared_ptr<Material> &material);
    ~Cone() override = default;

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

#endif  // SRC_PRIMITIVE_CONE_CONE_HPP_
