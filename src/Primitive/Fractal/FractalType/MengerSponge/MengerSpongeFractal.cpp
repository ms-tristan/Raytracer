// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Menger Sponge Fractal Type implementation
*/
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Primitive/Fractal/FractalType/MengerSponge/MengerSpongeFractal.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

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

Math::Vector3D estimateNormalMenger(const Math::Point3D& p, const Math::Point3D& center,
                          int maxIterations, double scale) {
    const double EPSILON = 0.0001;
    Math::Vector3D dx(Math::Coords{EPSILON, 0, 0});
    Math::Vector3D dy(Math::Coords{0, EPSILON, 0});
    Math::Vector3D dz(Math::Coords{0, 0, EPSILON});
    MengerSpongeFractal fractal(scale);
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

double MengerSpongeFractal::distanceEstimator(const Math::Point3D& point,
                                            const Math::Point3D& center,
                                            int maxIterations,
                                            double bailout,
                                            double power) const {
    (void)bailout;
    (void)power;
    Math::Vector3D p = point - center;
    p = p / scale;
    double d = boxDistance(p, Math::Vector3D(Math::Coords{1, 1, 1}));
    int iterations = std::min(maxIterations, 3);
    double m = 1.0;
    for (int i = 0; i < iterations; i++) {
        m *= 3.0;
        Math::Vector3D a = Math::Vector3D(Math::Coords{
            std::fmod(std::abs(p.X * m), 3.0),
            std::fmod(std::abs(p.Y * m), 3.0),
            std::fmod(std::abs(p.Z * m), 3.0)
        });
        Math::Vector3D r = Math::Vector3D(Math::Coords{
            a.X > 1.0 ? 3.0 - a.X : a.X,
            a.Y > 1.0 ? 3.0 - a.Y : a.Y,
            a.Z > 1.0 ? 3.0 - a.Z : a.Z
        });
        double s = 1.0;
        double v = 1.0;
        double crossDist = std::max(r.X, std::max(r.Y, r.Z)) - s;
        int count = (r.X > s ? 1 : 0) + (r.Y > s ? 1 : 0) + (r.Z > s ? 1 : 0);
        if (count >= 2) {
            d = std::max(d, crossDist / m);
        }
    }
    double finalDistance = d * 0.4;
    return std::max(0.0001, finalDistance);
}

}  // namespace RayTracer
