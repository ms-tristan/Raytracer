// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mandelbox Fractal Type implementation
*/
#include <cmath>
#include <algorithm>
#include "Primitive/Fractal/FractalType/Mandelbox/MandelboxFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

// Box folding function for mandelbox
Math::Vector3D boxFold(const Math::Vector3D& p, double foldingLimit) {
    Math::Vector3D result = p;
    
    if (result.X > foldingLimit) result.X = 2.0 * foldingLimit - result.X;
    else if (result.X < -foldingLimit) result.X = -2.0 * foldingLimit - result.X;
    
    if (result.Y > foldingLimit) result.Y = 2.0 * foldingLimit - result.Y;
    else if (result.Y < -foldingLimit) result.Y = -2.0 * foldingLimit - result.Y;
    
    if (result.Z > foldingLimit) result.Z = 2.0 * foldingLimit - result.Z;
    else if (result.Z < -foldingLimit) result.Z = -2.0 * foldingLimit - result.Z;
    
    return result;
}

// Sphere folding function for mandelbox
Math::Vector3D sphereFold(const Math::Vector3D& p, double minRadius2, double fixedRadius2) {
    double r2 = p.dot(p);
    Math::Vector3D result = p;
    
    // If r2 < minRadius2, the point is moved farther from the origin
    if (r2 < minRadius2) {
        double temp = fixedRadius2 / minRadius2;
        result = result * temp;
    } 
    // If minRadius2 < r2 < fixedRadius2, the point is moved closer to the origin
    else if (r2 < fixedRadius2) {
        double temp = fixedRadius2 / r2;
        result = result * temp;
    }
    
    return result;
}

double MandelboxFractal::distanceEstimator(const Math::Point3D& point,
                                         const Math::Point3D& center,
                                         int maxIterations,
                                         double bailout,
                                         double /*power*/) const {
    // Initialize position relative to center
    Math::Vector3D z = point - center;
    
    // Initialize derivative
    double dr = 1.0;
    
    // Fixed radius for sphere folding
    double fixedRadius2 = 1.0;
    
    // Iteration loop
    for (int i = 0; i < maxIterations; i++) {
        // Box folding - reflect points at boundaries
        z = boxFold(z, foldingLimit);
        
        // Sphere folding - expand points close to origin, contract points far away
        z = sphereFold(z, minRadius2, fixedRadius2);
        
        // Scale and translate
        z = z * scale + (point - center);
        
        // Update derivative for distance estimation
        dr = dr * std::abs(scale) + 1.0;
        
        // Escape condition
        if (z.length() > bailout) 
            break;
    }
    
    // Final distance calculation
    double distance = z.length() / std::abs(dr);
    
    return distance * 0.75; // Scaling factor for better visualization
}

}  // namespace RayTracer