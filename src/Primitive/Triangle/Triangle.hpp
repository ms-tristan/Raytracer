// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle Primitive
*/

#ifndef SRC_PRIMITIVE_TRIANGLE_TRIANGLE_HPP_
    #define SRC_PRIMITIVE_TRIANGLE_TRIANGLE_HPP_
    #include <string>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Primitive/APrimitive/APrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"
    #include <libconfig.h++>

namespace RayTracer {
class Triangle : public IPrimitive, public std::enable_shared_from_this<Triangle> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    std::string sourceFile = "";

 public:
    Math::Point3D vertex1;
    Math::Point3D vertex2;
    Math::Point3D vertex3;
    Math::Vector3D normal;

    Triangle(const Math::Point3D &v1, const Math::Point3D &v2, const Math::Point3D &v3);
    Triangle(const Math::Point3D &v1, const Math::Point3D &v2, const Math::Point3D &v3,
          const std::shared_ptr<Material> &material);
    ~Triangle() override = default;

    static std::string getTypeNameStatic() {
        return "triangles";
    }

    std::string getTypeName() const override {
        return Triangle::getTypeNameStatic();
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

#endif  // SRC_PRIMITIVE_TRIANGLE_TRIANGLE_HPP_
