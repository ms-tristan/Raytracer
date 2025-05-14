// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CompositePrimitive
*/

#ifndef SRC_PRIMITIVE_COMPOSITEPRIMITIVE_COMPOSITEPRIMITIVE_HPP_
    #define SRC_PRIMITIVE_COMPOSITEPRIMITIVE_COMPOSITEPRIMITIVE_HPP_
    #include <string>
    #include <vector>
    #include <memory>
    #include <libconfig.h++>
    #include "Primitive/IPrimitive.hpp"

namespace RayTracer {
class CompositePrimitive : public IPrimitive, public std::enable_shared_from_this<CompositePrimitive> {
 private:
    std::vector<std::shared_ptr<IPrimitive>> primitives;
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    std::string sourceFile = "";

 public:
    explicit CompositePrimitive(std::shared_ptr<Material> material);
    ~CompositePrimitive() override = default;

    static std::string getTypeNameStatic() {
        return "composites";
    }

    std::string getTypeName() const override {
        return CompositePrimitive::getTypeNameStatic();
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

    void add(std::shared_ptr<IPrimitive> primitive);
    void remove(std::shared_ptr<IPrimitive> primitive);
    const std::vector<std::shared_ptr<IPrimitive>>& getPrimitives() const;

    Math::Point3D getPosition() const override {
        if (primitives.empty())
            return Math::Point3D(Math::Coords{0.0, 0.0, 0.0});

        Math::Point3D average(Math::Coords{0.0, 0.0, 0.0});
        for (const auto& primitive : primitives) {
            Math::Point3D position = primitive->getPosition();
            average = average + Math::Vector3D(Math::Coords{position.X, position.Y, position.Z});
        }

        double count = static_cast<double>(primitives.size());
        return Math::Point3D(Math::Coords{
            average.X / count,
            average.Y / count,
            average.Z / count
        });
    }
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_COMPOSITEPRIMITIVE_COMPOSITEPRIMITIVE_HPP_
