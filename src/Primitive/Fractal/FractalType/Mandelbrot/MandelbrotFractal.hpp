// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mandelbrot Fractal Type
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MANDELBROT_MANDELBROTFRACTAL_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MANDELBROT_MANDELBROTFRACTAL_HPP_
    #include <complex>
    #include <memory>
    #include <string>
    #include "Primitive/Fractal/FractalType/IFractalType.hpp"

namespace RayTracer {

class MandelbrotFractal : public IFractalType {
 private:
    double xScale;
    double yScale;
    double zScale;

 public:
    explicit MandelbrotFractal(double xScale = 1.0, double yScale = 1.0,
        double zScale = 0.1)
        : xScale(xScale), yScale(yScale), zScale(zScale) {}
    ~MandelbrotFractal() override = default;

    std::string getName() const override {
        return "mandelbrot";
    }

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
        return std::make_shared<MandelbrotFractal>(*this);
    }
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MANDELBROT_MANDELBROTFRACTAL_HPP_
