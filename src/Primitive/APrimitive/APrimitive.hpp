// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** APrimitive implementation
*/

#ifndef SRC_PRIMITIVE_APRIMITIVE_APRIMITIVE_HPP_
#define SRC_PRIMITIVE_APRIMITIVE_APRIMITIVE_HPP_
#include <memory>
#include <string>
#include "../IPrimitive.hpp"

namespace RayTracer {
class APrimitive : public IPrimitive {
 protected:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    std::string sourceFile = "";

 public:
    APrimitive();
    explicit APrimitive(const std::shared_ptr<Material> &material);
    virtual ~APrimitive() = default;

    std::shared_ptr<Material> getMaterial() const override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;

    void setSourceFile(const std::string& source) override;
    std::string getSourceFile() const override;

    virtual void translate(const Math::Vector3D &translation) = 0;
    virtual std::optional<HitInfo> hit(const Ray &ray,
        double tMin, double tMax) = 0;
        
    virtual Math::Point3D getPosition() const override = 0;
};
}   // namespace RayTracer

#endif  // SRC_PRIMITIVE_APRIMITIVE_APRIMITIVE_HPP_
