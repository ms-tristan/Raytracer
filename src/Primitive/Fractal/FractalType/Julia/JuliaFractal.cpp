// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Julia Fractal Type implementation
*/
#include <cmath>
#include <complex>
#include <iostream>
#include "Primitive/Fractal/FractalType/Julia/JuliaFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

double JuliaFractal::distanceEstimator(const Math::Point3D& point,
                                      const Math::Point3D& center,
                                      int maxIterations,
                                      double bailout,
                                      double power) const {
    // Initialize position relative to center
    Math::Vector3D position = point - center;
    
    // Convert point coordinates to complex number for Julia iteration
    // Scale for better visualization
    std::complex<double> z(position.X * xScale, position.Y * yScale);
    
    // Get c parameter for Julia set from juliaConstant
    std::complex<double> c(juliaConstant.X, juliaConstant.Y);
    
    // The z coordinate represents height above/below the Julia plane
    double heightField = position.Z;
    
    // For derivative calculation
    std::complex<double> dz(1.0, 0.0);
    
    // Use proper power value, default to 2 if invalid
    double fractalPower = (power > 0.0) ? power : 2.0;
    
    // Track whether the point might be in the set
    bool mightBeInSet = true;
    
    // Julia iteration
    int i;
    for (i = 0; i < maxIterations; i++) {
        // Calculate derivative: dz = power * z^(power-1) * dz
        dz = fractalPower * std::pow(z, fractalPower - 1.0) * dz;
        
        // Julia iteration: z = z^power + c
        z = std::pow(z, fractalPower) + c;
        
        // Check for bailout
        if (std::abs(z) > bailout) {
            mightBeInSet = false;
            break;
        }
    }
    
    double distance;
    
    // If we reached max iterations, this point might be inside the set
    if (mightBeInSet) {
        // For points potentially in the set, return a small distance
        // Modulate with z-height to create a 3D effect
        double zFactor = std::abs(heightField) / zScale;
        distance = std::min(0.01, zFactor);
    } else {
        // For points outside the set, use distance estimation formula
        double r = std::abs(z);
        double dr = std::abs(dz);
        
        // Protect against division by zero
        if (dr < 1e-10) dr = 1e-10;
        
        // Standard distance estimation formula for Julia sets
        distance = 0.5 * std::log(r) * r / dr;
        
        // Apply a smoothing factor based on iteration count
        double smoothFactor = static_cast<double>(i) / maxIterations;
        distance *= (0.2 + 0.8 * smoothFactor);
    }
    
    // Scale distance based on the set parameters - smaller value = more detail
    distance *= 0.1;  
    
    // Combine with height field to create 3D effect
    double heightDistance = std::abs(heightField) / zScale;
    
    // Debug info for the first few calculations - commenting out to fix compilation errors
    /*
    static int debugCount = 0;
    if (debugCount < 5) {
        std::cout << "Julia DE at " << point.X << ", " << point.Y << ", " << point.Z 
                  << " with c=" << c.real() << "+" << c.imag() << "i"
                  << " = " << distance << std::endl;
        debugCount++;
    }
    */
    
    // Return the minimum distance, clamped for stability
    return std::max(0.0005, std::min(distance, heightDistance));
}

}  // namespace RayTracer