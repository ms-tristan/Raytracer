// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Fractal Primitive
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTAL_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTAL_HPP_
    #include <string>
    #include <memory>
    #include <complex>
    #include "Primitive/IPrimitive.hpp"
    #include "Transformation/Rotate/Rotate.hpp"
    #include "Primitive/Fractal/FractalType/IFractalType.hpp"
    #include "Primitive/Fractal/FractalType/FractalTypeFactory.hpp"
    #include <libconfig.h++>

namespace RayTracer {
class Fractal : public IPrimitive, public std::enable_shared_from_this<Fractal> {
 private:
    std::shared_ptr<Material> material;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    
    // Fractal specific parameters
    int maxIterations;
    double bailout;
    double power;
    std::shared_ptr<IFractalType> fractalType;
    
    // Ray marching algorithm for rendering the fractal
    std::optional<HitInfo> rayMarch(const Ray& ray, double tMin, double tMax) const;
    
    // Calculate the normal at a point using the distance estimator
    Math::Vector3D estimateNormal(const Math::Point3D& p) const;

 public:
    Math::Point3D center;
    double boundingRadius;
    
    // Constructors
    Fractal(const Math::Point3D &center, double boundingRadius, 
            const std::string &fractalTypeName, int maxIterations, double bailout);
    Fractal(const Math::Point3D &center, double boundingRadius,
            const std::string &fractalTypeName, int maxIterations, double bailout,
            const std::shared_ptr<Material> &material);
    ~Fractal() override = default;

    static std::string getTypeNameStatic() {
        return "fractals";
    }

    std::string getTypeName() const override {
        return Fractal::getTypeNameStatic();
    }
    
    // Fractal type methods
    void setFractalType(const std::string& name);
    std::string getFractalTypeName() const;
    std::shared_ptr<IFractalType> getFractalType() const;

    // Parameter setters
    void setPower(double p);
    void setMaxIterations(int iterations);
    void setBailout(double b);
    
    // Special parameter setters for specific fractal types
    void setJuliaConstant(const Math::Point3D& c);
    void setQuaternionConstant(double cx, double cy, double cz, double cw);
    void setMengerScale(double scale);
    void setSierpinskiParameters(double scale, bool useTetrahedron);
    void setMandelboxParameters(double scale, double minRadius, double foldingLimit);

    // Inherited methods from IPrimitive
    void translate(const Math::Vector3D &translation) override;
    void rotateX(double degrees) override;
    void rotateY(double degrees) override;
    void rotateZ(double degrees) override;
    std::optional<HitInfo> hit(const Ray &ray, double tMin, double tMax) override;
    std::shared_ptr<Material> getMaterial() const override;
    std::shared_ptr<IPrimitive> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTAL_HPP_