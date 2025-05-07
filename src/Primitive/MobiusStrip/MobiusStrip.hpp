// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mobius Strip Primitive
*/

#ifndef SRC_PRIMITIVE_MOBIUSSTRIP_MOBIUSSTRIP_HPP_
    #define SRC_PRIMITIVE_MOBIUSSTRIP_MOBIUSSTRIP_HPP_
    #include <string>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"
    #include <libconfig.h++>

namespace RayTracer {
class MobiusStrip : public IPrimitive, public std::enable_shared_from_this<MobiusStrip> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    std::string sourceFile = "";

 public:
    Math::Point3D center;   // Center of the Möbius strip
    double majorRadius;     // Distance from center to the middle of the strip
    double minorRadius;     // Half-width of the strip
    double thickness;       // Thickness of the strip

    MobiusStrip(const Math::Point3D &center, double majorRadius,
                double minorRadius, double thickness);
    MobiusStrip(const Math::Point3D &center, double majorRadius,
                double minorRadius, double thickness,
                const std::shared_ptr<Material> &material);
    ~MobiusStrip() override = default;

    static std::string getTypeNameStatic() {
        return "mobiusstrips";
    }

    std::string getTypeName() const override {
        return MobiusStrip::getTypeNameStatic();
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

#endif  // SRC_PRIMITIVE_MOBIUSSTRIP_MOBIUSSTRIP_HPP_
