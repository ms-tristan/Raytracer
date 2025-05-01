// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Menger Sponge Fractal Type
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MENGERSPONGE_MENGERSPONGEFRACTAL_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MENGERSPONGE_MENGERSPONGEFRACTAL_HPP_
    #include "Primitive/Fractal/FractalType/IFractalType.hpp"

namespace RayTracer {

class MengerSpongeFractal : public IFractalType {
 private:
    double scale;
    
 public:
    MengerSpongeFractal(double scale = 3.0)
        : scale(scale) {}
    ~MengerSpongeFractal() override = default;
    
    std::string getName() const override {
        return "menger_sponge";
    }
    
    void setScale(double s) {
        scale = s;
    }
    
    double getScale() const {
        return scale;
    }
    
    double distanceEstimator(const Math::Point3D& point,
                            const Math::Point3D& center,
                            int maxIterations,
                            double bailout,
                            double power) const override;
    
    std::shared_ptr<IFractalType> clone() const override {
        return std::make_shared<MengerSpongeFractal>(*this);
    }
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_MENGERSPONGE_MENGERSPONGEFRACTAL_HPP_