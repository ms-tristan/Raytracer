// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Quaternion Julia Fractal Type implementation
*/
#include <cmath>
#include <algorithm>
#include "Primitive/Fractal/FractalType/QuaternionJulia/QuaternionJuliaFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

double QuaternionJuliaFractal::distanceEstimator(const Math::Point3D& point,
                                               const Math::Point3D& center,
                                               int maxIterations,
                                               double bailout,
                                               double power) const {
    // Initialize position relative to center
    Math::Vector3D p = point - center;
    
    // Convert position to quaternion (using w = 0.0 for points in 3D space)
    Quaternion z(p.X, p.Y, p.Z, 0.0);
    
    // Initialize derivative for distance calculation
    Quaternion dz(1.0, 0.0, 0.0, 0.0);
    
    // Main iteration
    double r = 0.0;
    int i;
    for (i = 0; i < maxIterations; i++) {
        // Calculate derivative for distance estimation
        // Use the chain rule: dz' = power * z^(power-1) * dz
        dz = powerDerivative(z, dz, power);
        
        // Apply the iteration formula: z = z^power + c
        z = powerQuaternion(z, power) + constant;
        
        // Calculate magnitude for bailout check
        r = z.length();
        
        // Check if the point escapes
        if (r > bailout)
            break;
    }
    
    // Distance estimation formula
    double distance = 0.5 * r * std::log(r) / dz.length();
    
    // Ensure a stable and reasonable distance
    return std::max(0.001, distance);
}

// Helper function to calculate z^n for quaternions
Quaternion QuaternionJuliaFractal::powerQuaternion(const Quaternion& z, double n) const {
    // For quaternions, we use the polar form
    // z = |z| * (cos(θ) + u * sin(θ)) where u is the unit vector part
    
    double r = z.length();
    
    // Handle special cases
    if (r < 0.00001)
        return Quaternion(0, 0, 0, 0);
    
    // Extract the unit vector part
    double x = z.x / r;
    double y = z.y / r;
    double zz = z.z / r;  // using zz because z is already the quaternion name
    double w = z.w / r;
    
    // Calculate the angle
    double theta = std::acos(w);
    
    // Apply power via the polar form: r^n * (cos(n*θ) + u * sin(n*θ))
    double nTheta = n * theta;
    double rn = std::pow(r, n);
    double cosNTheta = std::cos(nTheta);
    double sinNTheta = std::sin(nTheta);
    
    // New quaternion after power operation
    return Quaternion(
        x * sinNTheta * rn,
        y * sinNTheta * rn,
        zz * sinNTheta * rn,
        cosNTheta * rn
    );
}

// Calculate derivative for quaternion power
Quaternion QuaternionJuliaFractal::powerDerivative(const Quaternion& z, const Quaternion& dz, double n) const {
    // We use a simpler approximation for quaternion derivative
    // This is a reasonable approximation for the Julia sets we're visualizing
    double r2 = z.dot(z);
    if (r2 < 0.00001)
        return dz;
    
    // Scale factor for the derivative
    double factor = n * std::pow(r2, (n - 1.0) / 2.0);
    
    // Multiply the current derivative by the scale factor
    return Quaternion(
        dz.x * factor,
        dz.y * factor,
        dz.z * factor,
        dz.w * factor
    );
}

}  // namespace RayTracer