// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Klein Bottle Primitive
*/

#ifndef SRC_PRIMITIVE_KLEINBOTTLE_KLEINBOTTLE_HPP_
    #define SRC_PRIMITIVE_KLEINBOTTLE_KLEINBOTTLE_HPP_
    #include <string>
    #include <memory>
    #include "Primitive/IPrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"
    #include <libconfig.h++>

namespace RayTracer {
class KleinBottle : public IPrimitive, public std::enable_shared_from_this<KleinBottle> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    bool isFigure8 = false;

 public:
    Math::Point3D center;
    double scale;
    double thickness;

    KleinBottle(const Math::Point3D &center, double scale, double thickness, bool isFigure8);
    KleinBottle(const Math::Point3D &center, double scale, double thickness, bool isFigure8,
                const std::shared_ptr<Material> &material);
    ~KleinBottle() override = default;

    static std::string getTypeNameStatic() {
        return "kleinbottles";
    }

    std::string getTypeName() const override {
        return KleinBottle::getTypeNameStatic();
    }

    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin, double tMax) override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;

 private:
    double distanceEstimator(const Math::Point3D &p) const;
    double classicBottleDE(const Math::Point3D &p) const;
    double figure8DE(const Math::Point3D &p) const;
    Math::Vector3D estimateNormal(const Math::Point3D &p) const;
    void applyRotations(std::shared_ptr<double> x, std::shared_ptr<double> y,
        std::shared_ptr<double> z) const;
    double mix(double a, double b, double t) const;
    double smoothstep(double edge0, double edge1, double x) const;
    double clamp(double x, double minVal, double maxVal) const;
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_KLEINBOTTLE_KLEINBOTTLE_HPP_
