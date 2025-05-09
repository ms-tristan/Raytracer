// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone Primitive
*/

#ifndef SRC_PRIMITIVE_CONE_CONE_HPP_
    #define SRC_PRIMITIVE_CONE_CONE_HPP_
    #include <string>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"
    #include <libconfig.h++>

namespace RayTracer {
class Cone : public IPrimitive, public std::enable_shared_from_this<Cone> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    std::string sourceFile = "";

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

    static std::string getTypeNameStatic() {
        return "cones";
    }

    std::string getTypeName() const override {
        return Cone::getTypeNameStatic();
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

#endif  // SRC_PRIMITIVE_CONE_CONE_HPP_
