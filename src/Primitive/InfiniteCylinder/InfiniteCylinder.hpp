/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InfiniteCylinder Primitive
*/

#ifndef SRC_PRIMITIVE_INFINITECYLINDER_INFINITECYLINDER_HPP_
    #define SRC_PRIMITIVE_INFINITECYLINDER_INFINITECYLINDER_HPP_

    #include <string>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"
    #include <libconfig.h++>

namespace RayTracer {
class InfiniteCylinder : public IPrimitive, public std::enable_shared_from_this<InfiniteCylinder> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    std::string sourceFile = "";

 public:
    Math::Point3D center;
    Math::Vector3D axis;
    double radius;

    InfiniteCylinder(const Math::Point3D &center, const Math::Vector3D &axis,
        double radius);
    InfiniteCylinder(const Math::Point3D &center, const Math::Vector3D &axis,
        double radius, const std::shared_ptr<Material> &material);
    ~InfiniteCylinder() override = default;

    static std::string getTypeNameStatic() {
        return "infinitecylinders";
    }

    std::string getTypeName() const override {
        return InfiniteCylinder::getTypeNameStatic();
    }

    void setSourceFile(const std::string& source) override {
        sourceFile = source;
    }

    std::string getSourceFile() const override {
        return sourceFile;
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
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_INFINITECYLINDER_INFINITECYLINDER_HPP_
