// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sierpinski Tetrahedron Fractal Type
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_SIERPINSKI_SIERPINSKITETRAHEDRONFRACTAL_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_SIERPINSKI_SIERPINSKITETRAHEDRONFRACTAL_HPP_
    #include <memory>
    #include <string>
    #include "Primitive/Fractal/FractalType/IFractalType.hpp"

namespace RayTracer {

class SierpinskiTetrahedronFractal : public IFractalType {
 private:
    double scale;
    double offset;

 public:
    explicit SierpinskiTetrahedronFractal(double scale = 2.0, double offset = 1.0)
        : scale(scale), offset(offset) {}
    ~SierpinskiTetrahedronFractal() override = default;
    std::string getName() const override {
        return "sierpinski_tetrahedron";
    }
    void setScale(double s) {
        scale = s;
    }
    double getScale() const {
        return scale;
    }
    void setOffset(double o) {
        offset = o;
    }
    double getOffset() const {
        return offset;
    }
    double distanceEstimator(const Math::Point3D& point,
                            const Math::Point3D& center,
                            int maxIterations,
                            double bailout,
                            double power) const override;
    std::shared_ptr<IFractalType> clone() const override {
        return std::make_shared<SierpinskiTetrahedronFractal>(*this);
    }
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_SIERPINSKI_SIERPINSKITETRAHEDRONFRACTAL_HPP_
