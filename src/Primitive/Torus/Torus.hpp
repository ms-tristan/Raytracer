/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus
*/

#ifndef SRC_PRIMITIVE_TORUS_TORUS_HPP_
#define SRC_PRIMITIVE_TORUS_TORUS_HPP_

#include <string>
#include <memory>
#include "Primitive/IPrimitive.hpp"
#include "Transformation/Rotate/Rotate.hpp"
#include <libconfig.h++>

namespace RayTracer {
class Torus : public IPrimitive, public std::enable_shared_from_this<Torus> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;

 public:
    Math::Point3D center;
    Math::Vector3D axis;
    double majorRadius;
    double minorRadius;

    Torus(const Math::Point3D &center, const Math::Vector3D &axis,
        double majorRadius, double minorRadius);
    Torus(const Math::Point3D &center, const Math::Vector3D &axis,
        double majorRadius, double minorRadius,
        const std::shared_ptr<Material> &material);
    ~Torus() override = default;

    static std::string getTypeNameStatic() {
        return "torus";
    }

    std::string getTypeName() const override {
        return Torus::getTypeNameStatic();
    }

    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    std::optional<HitInfo> hit(const Ray &ray,
        double tMin, double tMax) override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};
} // namespace RayTracer

#endif  // SRC_PRIMITIVE_TORUS_TORUS_HPP_