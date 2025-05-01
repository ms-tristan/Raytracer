// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Quaternion Julia Fractal Type
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_QUATERNIONJULIA_QUATERNIONJULIAFRACTAL_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_QUATERNIONJULIA_QUATERNIONJULIAFRACTAL_HPP_
    #include "Primitive/Fractal/FractalType/IFractalType.hpp"
    #include <array>

namespace RayTracer {

// Define a simple quaternion structure for the calculations
struct Quaternion {
    double x, y, z, w;
    
    Quaternion(double x = 0.0, double y = 0.0, double z = 0.0, double w = 0.0)
        : x(x), y(y), z(z), w(w) {}
    
    // Basic quaternion operations
    Quaternion operator+(const Quaternion& other) const {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }
    
    void operator*=(const Quaternion& other) {
        *this = *this * other;
    }
    
    Quaternion operator*(double scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    
    double dot(const Quaternion& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    
    double length() const {
        return std::sqrt(dot(*this));
    }
    
    Quaternion normalize() const {
        double len = length();
        if (len > 0.0) {
            return *this * (1.0 / len);
        }
        return *this;
    }
};

class QuaternionJuliaFractal : public IFractalType {
 private:
    Quaternion constant;  // The c parameter for the Quaternion Julia set
    
    // Helper functions for quaternion operations
    Quaternion powerQuaternion(const Quaternion& z, double n) const;
    Quaternion powerDerivative(const Quaternion& z, const Quaternion& dz, double n) const;
    
 public:
    QuaternionJuliaFractal(double cx = 0.35, double cy = 0.3, double cz = 0.0, double cw = 0.0)
        : constant(cx, cy, cz, cw) {}
    ~QuaternionJuliaFractal() override = default;
    
    std::string getName() const override {
        return "quaternion_julia";
    }
    
    // Setter and getter for the constant
    void setConstant(double cx, double cy, double cz, double cw) {
        constant = Quaternion(cx, cy, cz, cw);
    }
    
    Quaternion getConstant() const {
        return constant;
    }
    
    double distanceEstimator(const Math::Point3D& point,
                            const Math::Point3D& center,
                            int maxIterations,
                            double bailout,
                            double power) const override;
    
    std::shared_ptr<IFractalType> clone() const override {
        return std::make_shared<QuaternionJuliaFractal>(*this);
    }
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_QUATERNIONJULIA_QUATERNIONJULIAFRACTAL_HPP_