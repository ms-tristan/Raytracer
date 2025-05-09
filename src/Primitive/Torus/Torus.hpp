// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus Primitive
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
    std::string sourceFile = "";

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
        return getTypeNameStatic();
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
    std::optional<HitInfo> hit(const Ray &ray, double tMin, double tMax) override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;

 private:
    bool checkBoundingSphereIntersection(const Ray &ray) const;
    Ray transformRayForRotation(const Ray &ray) const;
    Math::Vector3D calculateNormal(const Math::Point3D &hitPoint) const;

    Math::Point3D sampleTorus(double u, double v) const {
        double phi = u * 2.0 * M_PI;
        double theta = v * 2.0 * M_PI;

        Math::Vector3D basisX, basisY;
        if (std::abs(axis.X) < 0.9) {
            basisX = axis.cross(Math::Vector3D(Math::Coords{1, 0, 0})).normalize();
        } else {
            basisX = axis.cross(Math::Vector3D(Math::Coords{0, 1, 0})).normalize();
        }
        basisY = axis.cross(basisX).normalize();

        Math::Point3D ringPoint = center +
            basisX * (majorRadius * std::cos(phi)) +
            basisY * (majorRadius * std::sin(phi));

        Math::Vector3D tubeOffset =
            basisX * (minorRadius * std::cos(theta) * std::cos(phi)) +
            basisY * (minorRadius * std::cos(theta) * std::sin(phi)) +
            axis * (minorRadius * std::sin(theta));

        return ringPoint + tubeOffset;
    }
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_TORUS_TORUS_HPP_
