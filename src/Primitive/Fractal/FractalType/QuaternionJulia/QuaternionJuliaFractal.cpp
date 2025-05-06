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
    Math::Vector3D p = point - center;
    Quaternion z(p.X, p.Y, p.Z, 0.0);
    Quaternion dz(1.0, 0.0, 0.0, 0.0);
    double r = 0.0;
    int i;
    for (i = 0; i < maxIterations; i++) {
        dz = powerDerivative(z, dz, power);
        z = powerQuaternion(z, power) + constant;
        r = z.length();
        if (r > bailout)
            break;
    }
    double distance = 0.5 * r * std::log(r) / dz.length();
    return std::max(0.001, distance);
}

Quaternion QuaternionJuliaFractal::powerQuaternion(const Quaternion& z, double n) const {
    double r = z.length();
    if (r < 0.00001)
        return Quaternion(0, 0, 0, 0);
    double x = z.x / r;
    double y = z.y / r;
    double zz = z.z / r;
    double w = z.w / r;
    double theta = std::acos(w);
    double nTheta = n * theta;
    double rn = std::pow(r, n);
    double cosNTheta = std::cos(nTheta);
    double sinNTheta = std::sin(nTheta);
    return Quaternion(
        x * sinNTheta * rn,
        y * sinNTheta * rn,
        zz * sinNTheta * rn,
        cosNTheta * rn);
}

Quaternion QuaternionJuliaFractal::powerDerivative(const Quaternion& z, const Quaternion& dz, double n) const {
    double r2 = z.dot(z);
    if (r2 < 0.00001)
        return dz;
    double factor = n * std::pow(r2, (n - 1.0) / 2.0);
    return Quaternion(
        dz.x * factor,
        dz.y * factor,
        dz.z * factor,
        dz.w * factor);
}

}  // namespace RayTracer
