// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sierpinski Tetrahedron Fractal Type implementation
*/
#include <cmath>
#include <algorithm>
#include <utility>
#include "Primitive/Fractal/FractalType/Sierpinski/SierpinskiTetrahedronFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

Math::Vector3D estimateNormalSierpinski(const Math::Point3D& p, const Math::Point3D& center,
                                int maxIterations, double scale, double offset) {
    const double EPSILON = 0.0001;
    Math::Vector3D dx(Math::Coords{EPSILON, 0, 0});
    Math::Vector3D dy(Math::Coords{0, EPSILON, 0});
    Math::Vector3D dz(Math::Coords{0, 0, EPSILON});
    SierpinskiTetrahedronFractal fractal(scale, offset);
    double gx = fractal.distanceEstimator(p + dx, center, maxIterations, 0, 0) -
              fractal.distanceEstimator(p - dx, center, maxIterations, 0, 0);
    double gy = fractal.distanceEstimator(p + dy, center, maxIterations, 0, 0) -
              fractal.distanceEstimator(p - dy, center, maxIterations, 0, 0);
    double gz = fractal.distanceEstimator(p + dz, center, maxIterations, 0, 0) -
              fractal.distanceEstimator(p - dz, center, maxIterations, 0, 0);
    Math::Vector3D normal(Math::Coords{gx, gy, gz});
    if (normal.length() < 1e-8) {
        return (center - p).normalize();
    }
    return normal.normalize();
}

double SierpinskiTetrahedronFractal::distanceEstimator(const Math::Point3D& point,
                                          const Math::Point3D& center,
                                          int maxIterations,
                                          double /*bailout*/,
                                          double /*power*/) const {
    Math::Vector3D p = point - center;
    double r = 2.0;
    Math::Vector3D z = p;
    int n = std::min(maxIterations, 9);
    for (int i = 0; i < n; i++) {
        z.X = std::fabs(z.X);
        z.Y = std::fabs(z.Y);
        z.Z = std::fabs(z.Z);
        if (z.X < z.Y) std::swap(z.X, z.Y);
        if (z.Y < z.Z) std::swap(z.Y, z.Z);
        if (z.X < z.Y) std::swap(z.X, z.Y);
        z.X = z.X * 2.0 - 1.0;
        z.X *= scale;
        z.Y *= scale;
        z.Z *= scale;
        r = r / scale;
    }
    double finalDistance = (Math::Vector3D(Math::Coords{z.Z, z.Y, 0.0}).length() - 0.5) * r;
    finalDistance *= 0.15;
    return std::max(0.0001, finalDistance);
}

}  // namespace RayTracer
