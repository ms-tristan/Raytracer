// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere Primitive
*/

#ifndef SRC_PRIMITIVE_SPHERE_SPHERE_HPP_
    #define SRC_PRIMITIVE_SPHERE_SPHERE_HPP_
    #include <string>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"
    #include <libconfig.h++>

namespace RayTracer {
class Sphere : public IPrimitive {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;

 public:
    Math::Point3D center;
    double radius;

    Sphere(const Math::Point3D &center, double radius);
    Sphere(const Math::Point3D &center, double radius,
        const std::shared_ptr<Material> &material);
    ~Sphere() override = default;

    static std::string getTypeNameStatic() {
        return "spheres";
    }

    std::string getTypeName() const override {
        return Sphere::getTypeNameStatic();
    }


    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin,
        double tMax) const override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
    void getLibConfigParams(libconfig::Setting& setting) const override;
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_SPHERE_SPHERE_HPP_
