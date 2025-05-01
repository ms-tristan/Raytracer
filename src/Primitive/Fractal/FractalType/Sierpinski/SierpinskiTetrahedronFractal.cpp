// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sierpinski Tetrahedron Fractal Type implementation
*/
#include <cmath>
#include <algorithm>
#include "Primitive/Fractal/FractalType/Sierpinski/SierpinskiTetrahedronFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

double SierpinskiTetrahedronFractal::distanceEstimator(const Math::Point3D& point,
                                          const Math::Point3D& center,
                                          int maxIterations,
                                          double bailout,
                                          double /*power*/) const {
    // Initialize position relative to center
    Math::Vector3D p = point - center;
    
    // Fixed iterations for Sierpinski (too many iterations can cause precision issues)
    int iterations = std::min(maxIterations, 10);
    
    // Initialize the distance
    double dr = 1.0;
    double r = p.length();
    
    // Iteration loop
    for (int i = 0; i < iterations; i++) {
        // Mirror the point if it's in negative space
        if (p.X + p.Y < 0) std::swap(p.X, p.Y);
        if (p.X + p.Z < 0) std::swap(p.X, p.Z);
        if (p.Y + p.Z < 0) std::swap(p.Y, p.Z);
        
        // Scale and translate the space to create the fractal pattern
        // Create a vector for the offset term
        Math::Vector3D offsetVec = Math::Vector3D(Math::Coords{
            offset * (scale - 1.0),
            offset * (scale - 1.0),
            offset * (scale - 1.0)
        });
        
        // Now perform p = p * scale - offsetVec
        p = Math::Vector3D(Math::Coords{
            p.X * scale - offsetVec.X,
            p.Y * scale - offsetVec.Y,
            p.Z * scale - offsetVec.Z
        });
        
        // Adjust for tetrahedron formula
        if (p.X < p.Y) std::swap(p.X, p.Y);
        if (p.X < p.Z) std::swap(p.X, p.Z);
        if (p.Y < p.Z) std::swap(p.Y, p.Z);
        
        // Update derivative for proper distance estimation
        dr = dr * scale;
    }
    
    // Calculate final distance
    r = p.length() / dr;
    
    return r * 0.5; // Adjust factor for better visualization
}

}  // namespace RayTracer