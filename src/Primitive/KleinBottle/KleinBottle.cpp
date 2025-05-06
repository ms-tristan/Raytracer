// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Klein Bottle Primitive implementation
*/

#include <cmath>
#include <memory>
#include <iostream>
#include <algorithm>
#include "KleinBottle.hpp"
#include "Math/Point3D/Point3D.hpp"

namespace RayTracer {

KleinBottle::KleinBottle(const Math::Point3D &center, double scale,
                         double thickness, bool isFigure8)
    : material(std::make_shared<Material>()), center(center),
      scale(scale), thickness(thickness), isFigure8(isFigure8) {}

KleinBottle::KleinBottle(const Math::Point3D &center, double scale,
                         double thickness, bool isFigure8,
                         const std::shared_ptr<Material> &material)
    : material(material), center(center), scale(scale),
      thickness(thickness), isFigure8(isFigure8) {}

void KleinBottle::translate(const Math::Vector3D &translation) {
    center = center + translation;
}

void KleinBottle::rotateX(double degrees) {
    rotationX += degrees;
}

void KleinBottle::rotateY(double degrees) {
    rotationY += degrees;
}

void KleinBottle::rotateZ(double degrees) {
    rotationZ += degrees;
}

std::shared_ptr<Material> KleinBottle::getMaterial() const {
    return material;
}

std::shared_ptr<IPrimitive> KleinBottle::clone() const {
    auto copy = std::make_shared<KleinBottle>(center, scale, thickness, isFigure8, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    return copy;
}

void KleinBottle::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    (*setting)["type"] = "kleinbottle";
    (*setting)["x"] = center.X;
    (*setting)["y"] = center.Y;
    (*setting)["z"] = center.Z;
    (*setting)["scale"] = scale;
    (*setting)["thickness"] = thickness;
    (*setting)["figure8"] = isFigure8 ? 1 : 0;
}

void KleinBottle::applyRotations(std::shared_ptr<double> x, std::shared_ptr<double> y, std::shared_ptr<double> z) const {
    if (rotationX != 0.0) {
        double rad = rotationX * M_PI / 180.0;
        double newY = *y * cos(rad) - *z * sin(rad);
        double newZ = *y * sin(rad) + *z * cos(rad);
        *y = newY;
        *z = newZ;
    }
    if (rotationY != 0.0) {
        double rad = rotationY * M_PI / 180.0;
        double newX = *x * cos(rad) + *z * sin(rad);
        double newZ = -*x * sin(rad) + *z * cos(rad);
        *x = newX;
        *z = newZ;
    }
    if (rotationZ != 0.0) {
        double rad = rotationZ * M_PI / 180.0;
        double newX = *x * cos(rad) - *y * sin(rad);
        double newY = *x * sin(rad) + *y * cos(rad);
        *x = newX;
        *y = newY;
    }
}

double KleinBottle::classicBottleDE(const Math::Point3D &p) const {
    auto x = std::make_shared<double>((p.X - center.X) / scale);
    auto y = std::make_shared<double>((p.Y - center.Y) / scale);
    auto z = std::make_shared<double>((p.Z - center.Z) / scale);

    applyRotations(x, y, z);

    double a = 0.8;
    double b = 0.3;
    double r = std::sqrt((*x)*(*x) + (*y)*(*y));
    double d = 1e10;
    for (int i = 0; i < 16; i++) {
        double u = i * M_PI / 8.0;
        for (int j = 0; j < 16; j++) {
            double v = j * M_PI / 8.0;
            double kx, ky, kz;

            if (u < M_PI) {
                kx = a * (cos(u) * (1 + cos(v)) + b * cos(u) * cos(v));
                ky = a * (sin(u) * (1 + cos(v)) + b * sin(u) * cos(v));
                kz = b * sin(v);
            } else {
                kx = a * (cos(u) * (1 + cos(v)) - b * cos(u) * cos(v));
                ky = a * (sin(u) * (1 + cos(v)) - b * sin(u) * cos(v));
                kz = -b * sin(v);
            }
            double dx = *x - kx;
            double dy = *y - ky;
            double dz = *z - kz;
            double dist = std::sqrt(dx*dx + dy*dy + dz*dz);
            d = std::min(d, dist);
        }
    }
    return d - thickness / scale;
}

double KleinBottle::figure8DE(const Math::Point3D &p) const {
    auto x = std::make_shared<double>((p.X - center.X) / scale);
    auto y = std::make_shared<double>((p.Y - center.Y) / scale);
    auto z = std::make_shared<double>((p.Z - center.Z) / scale);

    applyRotations(x, y, z);

    double r = 1.0;
    double a = 0.2;
    double rho = sqrt((*x)*(*x) + (*y)*(*y));
    double phi = atan2(*y, *x);
    double u = phi / 2.0;
    double v = atan2(*z, rho - r);
    double cx = r * cos(u) * (cos(v) + 1.0) / 2.0;
    double cy = r * sin(u) * (cos(v) + 1.0) / 2.0;
    double cz = r * sin(v) / 2.0;
    double dx = rho * cos(phi) - cx;
    double dy = rho * sin(phi) - cy;
    double dz = *z - cz;
    double d = sqrt(dx*dx + dy*dy + dz*dz) - a;
    d -= 0.05 * sin(2.0 * phi) * sin(v);
    return d * scale - thickness;
}

double KleinBottle::distanceEstimator(const Math::Point3D &p) const {
    return isFigure8 ? figure8DE(p) : classicBottleDE(p);
}

double KleinBottle::mix(double a, double b, double t) const {
    return a * (1.0 - t) + b * t;
}

double KleinBottle::clamp(double x, double minVal, double maxVal) const {
    return std::min(std::max(x, minVal), maxVal);
}

double KleinBottle::smoothstep(double edge0, double edge1, double x) const {
    double t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

Math::Vector3D KleinBottle::estimateNormal(const Math::Point3D &p) const {
    constexpr double EPSILON = 1e-4;
    double dx = distanceEstimator(Math::Point3D(Math::Coords{p.X + EPSILON, p.Y, p.Z})) -
                distanceEstimator(Math::Point3D(Math::Coords{p.X - EPSILON, p.Y, p.Z}));
    double dy = distanceEstimator(Math::Point3D(Math::Coords{p.X, p.Y + EPSILON, p.Z})) -
                distanceEstimator(Math::Point3D(Math::Coords{p.X, p.Y - EPSILON, p.Z}));
    double dz = distanceEstimator(Math::Point3D(Math::Coords{p.X, p.Y, p.Z + EPSILON})) -
                distanceEstimator(Math::Point3D(Math::Coords{p.X, p.Y, p.Z - EPSILON}));

    return Math::Vector3D(Math::Coords{dx, dy, dz}).normalize();
}

std::optional<HitInfo> KleinBottle::hit(const Ray &ray, double tMin, double tMax) {
    double t = tMin;
    const double minDistance = 0.001;
    const int maxSteps = 256;
    const double maxStepSize = 0.1;

    for (int i = 0; i < maxSteps && t < tMax; i++) {
        Math::Point3D position = ray.origin + ray.direction * t;
        double distance = distanceEstimator(position);

        if (distance < minDistance) {
            HitInfo info;
            info.distance = t;
            info.hitPoint = position;
            info.normal = estimateNormal(position);
            info.primitive = shared_from_this();
            return info;
        }
        t += std::max(distance, 0.0001);
    }
    return std::nullopt;
}

} // namespace RayTracer
