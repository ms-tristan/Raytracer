// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mandelbox Fractal Type
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MANDELBOX_MANDELBOXFRACTAL_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MANDELBOX_MANDELBOXFRACTAL_HPP_
    #include <string>
    #include <memory>
    #include "Primitive/Fractal/FractalType/IFractalType.hpp"

namespace RayTracer {

class MandelboxFractal : public IFractalType {
 private:
    double scale;
    double minRadius2;
    double foldingLimit;

 public:
    explicit MandelboxFractal(double scale = 2.0, double minRadius2 = 0.25, double foldingLimit = 1.0)
        : scale(scale), minRadius2(minRadius2), foldingLimit(foldingLimit) {}
    ~MandelboxFractal() override = default;

    std::string getName() const override {
        return "mandelbox";
    }
    void setScale(double s) {
        scale = s;
    }
    void setMinRadius2(double r2) {
        minRadius2 = r2;
    }
    void setFoldingLimit(double limit) {
        foldingLimit = limit;
    }
    double getScale() const {
        return scale;
    }
    double getMinRadius2() const {
        return minRadius2;
    }
    double getFoldingLimit() const {
        return foldingLimit;
    }
    double distanceEstimator(const Math::Point3D& point,
                            const Math::Point3D& center,
                            int maxIterations,
                            double bailout,
                            double power) const override;
    std::shared_ptr<IFractalType> clone() const override {
        return std::make_shared<MandelboxFractal>(*this);
    }
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MANDELBOX_MANDELBOXFRACTAL_HPP_
