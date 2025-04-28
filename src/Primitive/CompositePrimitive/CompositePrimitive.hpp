// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CompositePrimitive
*/

#ifndef SRC_PRIMITIVE_COMPOSITEPRIMITIVE_COMPOSITEPRIMITIVE_HPP_
    #define SRC_PRIMITIVE_COMPOSITEPRIMITIVE_COMPOSITEPRIMITIVE_HPP_

    #include "Primitive/IPrimitive.hpp"

    #include <vector>
    #include <memory>
    #include <libconfig.h++>

namespace RayTracer {
class CompositePrimitive : public IPrimitive {
 private:
    std::vector<std::shared_ptr<IPrimitive>> primitives;
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;

 public:
    explicit CompositePrimitive(std::shared_ptr<Material> material);
    ~CompositePrimitive() override = default;
    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    std::optional<HitInfo> hit(const Ray &ray,
        double tMin, double tMax) const override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
    void getLibConfigParams(libconfig::Setting& setting) const override;
    void add(std::shared_ptr<IPrimitive> primitive);
    void remove(std::shared_ptr<IPrimitive> primitive);
    const std::vector<std::shared_ptr<IPrimitive>>& getPrimitives() const;
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_COMPOSITEPRIMITIVE_COMPOSITEPRIMITIVE_HPP_
