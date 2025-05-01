// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Julia Fractal Type
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_JULIA_JULIAFRACTAL_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_JULIA_JULIAFRACTAL_HPP_
    #include "Primitive/Fractal/FractalType/IFractalType.hpp"
    #include <complex>

namespace RayTracer {

class JuliaFractal : public IFractalType {
 private:
    Math::Point3D juliaConstant;  // The c parameter for the Julia set
    double xScale;                // Scale for mapping x coordinate
    double yScale;                // Scale for mapping y coordinate
    double zScale;                // Scale for thickness in z direction
    
 public:
    JuliaFractal(const Math::Point3D &constant = Math::Point3D(Math::Coords{-0.8, 0.156, 0}),
                double xScale = 1.0, double yScale = 1.0, double zScale = 0.1)
        : juliaConstant(constant), xScale(xScale), yScale(yScale), zScale(zScale) {}
    ~JuliaFractal() override = default;
    
    std::string getName() const override {
        return "julia";
    }
    
    // Parameter setters and getters
    void setJuliaConstant(const Math::Point3D &c) {
        juliaConstant = c;
    }
    
    Math::Point3D getJuliaConstant() const {
        return juliaConstant;
    }
    
    // Scale setters and getters
    void setXScale(double scale) { xScale = scale; }
    void setYScale(double scale) { yScale = scale; }
    void setZScale(double scale) { zScale = scale; }
    
    double getXScale() const { return xScale; }
    double getYScale() const { return yScale; }
    double getZScale() const { return zScale; }
    
    double distanceEstimator(const Math::Point3D& point,
                            const Math::Point3D& center,
                            int maxIterations,
                            double bailout,
                            double power) const override;
    
    std::shared_ptr<IFractalType> clone() const override {
        return std::make_shared<JuliaFractal>(*this);
    }
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_JULIA_JULIAFRACTAL_HPP_