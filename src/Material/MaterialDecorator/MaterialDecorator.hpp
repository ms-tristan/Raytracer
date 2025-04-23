// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** MaterialDecorator
*/

#ifndef SRC_MATERIAL_MATERIALDECORATOR_MATERIALDECORATOR_HPP_
    #define SRC_MATERIAL_MATERIALDECORATOR_MATERIALDECORATOR_HPP_
    #include <memory>
    #include "Primitive/PrimitiveDecorator/PrimitiveDecorator.hpp"

namespace RayTracer {
class MaterialDecorator : public PrimitiveDecorator {
 private:
    std::shared_ptr<Material> overrideMaterial;

 public:
    MaterialDecorator(std::shared_ptr<IPrimitive> primitive,
        std::shared_ptr<Material> material);
    ~MaterialDecorator() override = default;

    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
};
}  // namespace RayTracer

#endif  // SRC_MATERIAL_MATERIALDECORATOR_MATERIALDECORATOR_HPP_
