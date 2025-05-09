// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Box
*/

#ifndef SRC_PRIMITIVE_BOX_BOX_HPP_
    #define SRC_PRIMITIVE_BOX_BOX_HPP_
    #include <string>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"

namespace RayTracer {
class Box : public IPrimitive, public std::enable_shared_from_this<Box> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    std::string sourceFile = "";

 public:
    Math::Point3D center;
    Math::Vector3D dimensions;

    Box(const Math::Point3D &center, const Math::Vector3D &dimensions);
    Box(const Math::Point3D &center, const Math::Vector3D &dimensions,
        const std::shared_ptr<Material> &material);
    ~Box() override = default;

    static std::string getTypeNameStatic() {
        return "boxes";
    }

    std::string getTypeName() const override {
        return Box::getTypeNameStatic();
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

#endif  // SRC_PRIMITIVE_BOX_BOX_HPP_
