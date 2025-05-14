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
    #include <string>
    #include <libconfig.h++>
    #include "Primitive/IPrimitive.hpp"

namespace RayTracer {
class PrimitiveDecorator : public IPrimitive, public std::enable_shared_from_this<PrimitiveDecorator> {
 protected:
    std::shared_ptr<IPrimitive> wrappedPrimitive;

 public:
    explicit PrimitiveDecorator(std::shared_ptr<IPrimitive> primitive);
    ~PrimitiveDecorator() override = default;

    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    void setSourceFile(const std::string& source) override;
    std::string getSourceFile() const override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin,
        double tMax) override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override = 0;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;

    Math::Point3D getPosition() const override { return wrappedPrimitive->getPosition(); }
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_PRIMITIVEDECORATOR_PRIMITIVEDECORATOR_HPP_
