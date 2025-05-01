// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mandelbrot Fractal Type implementation
*/
#include <cmath>
#include <complex>
#include "Primitive/Fractal/FractalType/Mandelbrot/MandelbrotFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

double MandelbrotFractal::distanceEstimator(const Math::Point3D& point,
                                          const Math::Point3D& center,
                                          int maxIterations,
                                          double bailout,
                                          double power) const {
    // Initialize position relative to center
    Math::Vector3D position = point - center;
    
    // Use x and y to map to the complex plane
    // Scale the coordinates for better visualization
    std::complex<double> c(position.X * xScale, position.Y * yScale);
    
    // The z coordinate represents height above/below the Mandelbrot plane
    double heightField = position.Z;
    
    // Initialize z for Mandelbrot iteration
    std::complex<double> z(0.0, 0.0);
    
    // For derivative calculation
    std::complex<double> dz(1.0, 0.0);
    
    // Use proper power value, default to 2 if invalid
    double fractalPower = (power > 0.0) ? power : 2.0;
    
    // Track whether the point might be in the set
    bool mightBeInSet = true;
    
    // Mandelbrot iteration
    int i;
    for (i = 0; i < maxIterations; i++) {
        // Calculate derivative for distance estimation: dz = power * z^(power-1) * dz + 1
        dz = fractalPower * std::pow(z, fractalPower - 1.0) * dz + 1.0;
        
        // Standard Mandelbrot iteration: z = z^power + c
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
        // For points outside the set, use the standard distance estimation formula
        double r = std::abs(z);
        double dr = std::abs(dz);
        
        // Improved distance estimation formula for Mandelbrot set
        // Use logarithmic scaling for smoother results
        distance = 0.5 * std::log(r) * r / dr;
        
        // Apply a smoothing factor based on the bailout and iteration count
        double smoothFactor = static_cast<double>(i) / maxIterations;
        distance *= (0.5 + 0.5 * smoothFactor);
    }
    
    // Scale distance based on the set parameters
    distance *= 0.2;
    
    // Combine with height field to create 3D effect
    double heightDistance = std::abs(heightField) / zScale;
    
    // Return the minimum distance, clamped for stability
    return std::max(0.001, std::min(distance, heightDistance));
}

}  // namespace RayTracer