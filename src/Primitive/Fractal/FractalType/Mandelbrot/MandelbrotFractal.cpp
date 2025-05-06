/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** MandelBrot
*/
#include <cmath>
#include <iostream>
#include <complex>
#include <algorithm>
#include "Primitive/Fractal/FractalType/Mandelbrot/MandelbrotFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

double MandelbrotFractal::distanceEstimator(const Math::Point3D& point,
                                          const Math::Point3D& center,
                                          int maxIterations,
                                          double bailout,
                                          double power) const {
    Math::Vector3D position = point - center;
    std::complex<double> c(position.X * xScale, position.Y * yScale);
    double heightField = position.Z;
    std::complex<double> z(0.0, 0.0);
    std::complex<double> dz(1.0, 0.0);
    double fractalPower = (power > 0.0) ? power : 2.0;
    bool mightBeInSet = true;

    int i;
    for (i = 0; i < maxIterations; i++) {
        dz = fractalPower * std::pow(z, fractalPower - 1.0) * dz + 1.0;
        z = std::pow(z, fractalPower) + c;
        if (std::abs(z) > bailout) {
            mightBeInSet = false;
            break;
        }
    }

    double distance;

    if (mightBeInSet) {
        distance = 0.0001;
    } else {
        double r = std::abs(z);
        double dr = std::abs(dz);
        if (dr < 1e-10) dr = 1e-10;
        distance = 0.5 * std::log(r) * r / dr;
        double smoothFactor = static_cast<double>(i) / maxIterations;
        distance *= (0.2 + 0.8 * smoothFactor);
    }
    distance *= 0.02;
    double heightDistance = std::abs(heightField) / zScale;
    return std::max(0.00001, std::min(distance, heightDistance));
}
}  // namespace RayTracer
