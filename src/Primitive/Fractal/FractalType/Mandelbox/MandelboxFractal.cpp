// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mandelbox Fractal Type implementation
*/
#include <cmath>
#include <iostream>
#include <algorithm>
#include "Primitive/Fractal/FractalType/Mandelbox/MandelboxFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {
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

Math::Vector3D sphereFold(const Math::Vector3D& p, double minRadius2, double fixedRadius2) {
    double r2 = p.dot(p);
    Math::Vector3D result = p;
    if (r2 < minRadius2) {
        double temp = fixedRadius2 / minRadius2;
        result = result * temp;
    } else if (r2 < fixedRadius2) {
        double temp = fixedRadius2 / r2;
        result = result * temp;
    }
    return result;
}

double MandelboxFractal::distanceEstimator(const Math::Point3D& point,
                                         const Math::Point3D& center,
                                         int maxIterations,
                                         double bailout,
                                         double) const {
    Math::Vector3D z = point - center;
    double dr = 1.0;
    double fixedRadius2 = 1.0;

    for (int i = 0; i < maxIterations; i++) {
        z = boxFold(z, foldingLimit);
        z = sphereFold(z, minRadius2, fixedRadius2);
        z = z * scale + (point - center) * 0.2;
        dr = dr * std::abs(scale) + 1.0;
        if (z.length() > bailout)
            break;
    }
    if (dr < 1e-10) dr = 1e-10;
    double distance = z.length() / std::abs(dr);
    distance *= 0.02;
    return std::max(0.00001, distance);
}

}  // namespace RayTracer
