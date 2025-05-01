// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Menger Sponge Fractal Type implementation
*/
#include <cmath>
#include <algorithm>
#include "Primitive/Fractal/FractalType/MengerSponge/MengerSpongeFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

// Helper function for the Menger sponge - distance to a box
double boxDistance(const Math::Vector3D& p, const Math::Vector3D& b) {
    Math::Vector3D d = Math::Vector3D(Math::Coords{
        std::abs(p.X) - b.X,
        std::abs(p.Y) - b.Y,
        std::abs(p.Z) - b.Z
    });
    
    return std::min(std::max(d.X, std::max(d.Y, d.Z)), 0.0) +
           Math::Vector3D(Math::Coords{
               std::max(d.X, 0.0),
               std::max(d.Y, 0.0),
               std::max(d.Z, 0.0)
           }).length();
}

// Main distance estimator for Menger sponge
double MengerSpongeFractal::distanceEstimator(const Math::Point3D& point,
                                            const Math::Point3D& center,
                                            int maxIterations,
                                            double /*bailout*/,
                                            double /*power*/) const {
    // Initialize position relative to center
    Math::Vector3D p = point - center;
    
    // Limit max iterations for the Menger sponge to prevent precision issues
    int iterations = std::min(maxIterations, 6);
    
    // Distance to outer box (the basic shape of Menger sponge is a cube)
    double d = boxDistance(p, Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}));
    
    double s = 1.0;
    
    // Iterate to create the fractal detail
    for (int i = 0; i < iterations; i++) {
        // Scale coordinates by 3 for the standard Menger sponge
        s *= 3.0 / scale;
        
        // Take absolute value and create the cross pattern
        Math::Vector3D a = Math::Vector3D(Math::Coords{
            std::fmod(std::abs(p.X * s), 3.0),
            std::fmod(std::abs(p.Y * s), 3.0),
            std::fmod(std::abs(p.Z * s), 3.0)
        });
        
        // Subtract 1.5 from each component individually
        a.X -= 1.5;
        a.Y -= 1.5;
        a.Z -= 1.5;
        
        // Calculate the cross pattern of the sponge
        d = std::max(d, boxDistance(a, Math::Vector3D(Math::Coords{1.5, 1.5, 1.5})) / s);
        
        // Cross calculation - this removes the "crosses" from the cube
        Math::Vector3D r = Math::Vector3D(Math::Coords{
            std::max(a.X - 1.0, 0.0),
            std::max(a.Y - 1.0, 0.0),
            std::max(a.Z - 1.0, 0.0)
        });
        
        double crossDistance = std::min(
            std::max(r.X, std::max(r.Y, r.Z)),
            0.0
        );
        
        d = std::max(d, -crossDistance / s);
    }
    
    return d * 0.5; // Scale factor for better visualization
}

}  // namespace RayTracer