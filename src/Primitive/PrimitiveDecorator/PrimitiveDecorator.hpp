// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrimitiveDecorator
*/

#ifndef SRC_PRIMITIVE_PRIMITIVEDECORATOR_PRIMITIVEDECORATOR_HPP_
    #define SRC_PRIMITIVE_PRIMITIVEDECORATOR_PRIMITIVEDECORATOR_HPP_
    #include <memory>
    #include "Primitive/IPrimitive.hpp"

namespace RayTracer {
class PrimitiveDecorator : public IPrimitive {
 protected:
    std::shared_ptr<IPrimitive> wrappedPrimitive;

 public:
    explicit PrimitiveDecorator(std::shared_ptr<IPrimitive> primitive);
    ~PrimitiveDecorator() override = default;

    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin,
        double tMax) const override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override = 0;
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_PRIMITIVEDECORATOR_PRIMITIVEDECORATOR_HPP_
