// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Fractal implementation
*/
#include <cmath>
#include <memory>
#include <algorithm>
#include <complex>
#include <iostream>
#include <string>
#include <limits>
#include "Primitive/Fractal/Fractal.hpp"
#include "Primitive/Fractal/FractalType/Julia/JuliaFractal.hpp"
#include "Primitive/Fractal/FractalType/Mandelbox/MandelboxFractal.hpp"
#include "Primitive/Fractal/FractalType/MengerSponge/MengerSpongeFractal.hpp"
#include "Primitive/Fractal/FractalType/Sierpinski/SierpinskiTetrahedronFractal.hpp"
#include "Primitive/Fractal/FractalType/QuaternionJulia/QuaternionJuliaFractal.hpp"

namespace RayTracer {

Fractal::Fractal(const Math::Point3D &center, double boundingRadius,
                 const std::string &fractalTypeName, int maxIterations, double bailout)
    : material(std::make_shared<Material>()), center(center),
      boundingRadius(boundingRadius), maxIterations(maxIterations),
      bailout(bailout), power(2.0) {
    fractalType = FractalTypeFactory::getInstance().createFractalType(fractalTypeName);
}

Fractal::Fractal(const Math::Point3D &center, double boundingRadius,
                 const std::string &fractalTypeName, int maxIterations, double bailout,
                 const std::shared_ptr<Material> &material)
    : material(material), center(center), boundingRadius(boundingRadius),
      maxIterations(maxIterations), bailout(bailout), power(2.0) {
    fractalType = FractalTypeFactory::getInstance().createFractalType(fractalTypeName);
}

void Fractal::setFractalType(const std::string& name) {
    fractalType = FractalTypeFactory::getInstance().createFractalType(name);
}

std::string Fractal::getFractalTypeName() const {
    return fractalType->getName();
}

std::shared_ptr<IFractalType> Fractal::getFractalType() const {
    return fractalType;
}

void Fractal::setPower(double p) {
    power = p;
}

void Fractal::setMaxIterations(int iterations) {
    maxIterations = iterations;
}

void Fractal::setBailout(double b) {
    bailout = b;
}

void Fractal::setJuliaConstant(const Math::Point3D& c) {
    auto juliaFractal = std::dynamic_pointer_cast<JuliaFractal>(fractalType);
    if (juliaFractal) {
        juliaFractal->setJuliaConstant(c);
    }
}

void Fractal::setQuaternionConstant(double cx, double cy, double cz, double cw) {
    auto quaternionFractal = std::dynamic_pointer_cast<QuaternionJuliaFractal>(fractalType);
    if (quaternionFractal) {
        quaternionFractal->setConstant(cx, cy, cz, cw);
    }
}

void Fractal::setMengerScale(double scale) {
    auto mengerFractal = std::dynamic_pointer_cast<MengerSpongeFractal>(fractalType);
    if (mengerFractal) {
        mengerFractal->setScale(scale);
    }
}

void Fractal::setSierpinskiParameters(double scale, bool useTetrahedron) {
    auto sierpinskiFractal = std::dynamic_pointer_cast<SierpinskiTetrahedronFractal>(fractalType);
    if (sierpinskiFractal) {
        sierpinskiFractal->setScale(scale);
    }
}

void Fractal::setMandelboxParameters(double scale, double minRadius, double foldingLimit) {
    auto mandelboxFractal = std::dynamic_pointer_cast<MandelboxFractal>(fractalType);
    if (mandelboxFractal) {
        mandelboxFractal->setScale(scale);
        mandelboxFractal->setMinRadius2(minRadius);
        mandelboxFractal->setFoldingLimit(foldingLimit);
    }
}

void Fractal::translate(const Math::Vector3D &translation) {
    center += translation;
}

void Fractal::rotateX(double degrees) {
    rotationX += degrees;
    RayTracer::Rotate rotateX("x", degrees);
    center = rotateX.applyToPoint(center);
}

void Fractal::rotateY(double degrees) {
    rotationY += degrees;
    RayTracer::Rotate rotateY("y", degrees);
    center = rotateY.applyToPoint(center);
}

void Fractal::rotateZ(double degrees) {
    rotationZ += degrees;
    RayTracer::Rotate rotateZ("z", degrees);
    center = rotateZ.applyToPoint(center);
}

std::optional<HitInfo> Fractal::hit(const Ray &ray, double tMin, double tMax) {
    Ray transformedRay = ray;
    if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
        Math::Point3D newOrigin = ray.origin;
        Math::Vector3D newDirection = ray.direction;

        if (rotationZ != 0.0) {
            RayTracer::Rotate rotateZ("z", -rotationZ);
            newOrigin = rotateZ.applyToPoint(newOrigin);
            newDirection = rotateZ.applyToVector(newDirection);
        }
        if (rotationY != 0.0) {
            RayTracer::Rotate rotateY("y", -rotationY);
            newOrigin = rotateY.applyToPoint(newOrigin);
            newDirection = rotateY.applyToVector(newDirection);
        }
        if (rotationX != 0.0) {
            RayTracer::Rotate rotateX("x", -rotationX);
            newOrigin = rotateX.applyToPoint(newOrigin);
            newDirection = rotateX.applyToVector(newDirection);
        }

        transformedRay = Ray(newOrigin, newDirection);
    }

    Math::Vector3D oc = transformedRay.origin - center;
    double a = transformedRay.direction.dot(transformedRay.direction);
    double b = 2.0 * oc.dot(transformedRay.direction);
    double c = oc.dot(oc) - boundingRadius * boundingRadius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return std::nullopt;
    double sqrtd = std::sqrt(discriminant);
    double t1 = (-b - sqrtd) / (2 * a);
    double t2 = (-b + sqrtd) / (2 * a);
    double entryT = (t1 > tMin) ? t1 : tMin;
    double exitT = (t2 < tMax) ? t2 : tMax;
    if (entryT > exitT)
        return std::nullopt;
    return rayMarch(transformedRay, entryT, exitT);
}

std::optional<HitInfo> Fractal::rayMarch(const Ray& ray, double tMin, double tMax) {
    const double PRECISION = 0.00001;
    const int MAX_STEPS = 500;
    const double EPSILON = 0.0001 * boundingRadius;
    double t = tMin;
    double lastDistance = std::numeric_limits<double>::max();
    int stepsSinceReduction = 0;
    for (int i = 0; i < MAX_STEPS && t < tMax; i++) {
        Math::Point3D point = ray.origin + ray.direction * t;
        double distance = fractalType->distanceEstimator(point, center, maxIterations, bailout, power);
        distance = std::max(distance, 1e-12);
        double stepFactor = 0.5;
        if (distance < EPSILON * 10.0) {
            stepFactor = 0.1;
            stepsSinceReduction = 0;
        }
        if (distance < EPSILON) {
            HitInfo info;
            info.distance = t;
            info.hitPoint = ray.origin + ray.direction * t;
            info.normal = estimateNormal(info.hitPoint);
            info.normal = info.normal.normalize();
            info.primitive = std::static_pointer_cast<IPrimitive>(shared_from_this());
            if (rotationX != 0.0 || rotationY != 0.0 || rotationZ != 0.0) {
                if (rotationX != 0.0) {
                    RayTracer::Rotate rotateX("x", rotationX);
                    info.normal = rotateX.applyToVector(info.normal);
                }
                if (rotationY != 0.0) {
                    RayTracer::Rotate rotateY("y", rotationY);
                    info.normal = rotateY.applyToVector(info.normal);
                }
                if (rotationZ != 0.0) {
                    RayTracer::Rotate rotateZ("z", rotationZ);
                    info.normal = rotateZ.applyToVector(info.normal);
                }
                info.normal = info.normal.normalize();
            }
            return info;
        }
        if (distance < lastDistance) {
            stepsSinceReduction++;
            if (stepsSinceReduction > 5) {
                stepFactor = std::min(0.7, stepFactor * 1.1);
            }
        } else {
            stepFactor *= 0.5;
            stepsSinceReduction = 0;
        }
        lastDistance = distance;
        double step = distance * stepFactor;
        if (step < PRECISION) {
            step = PRECISION;
        }
        t += step;
    }
    return std::nullopt;
}

Math::Vector3D Fractal::estimateNormal(const Math::Point3D& p) const {
    const double EPSILON = 0.00005 * boundingRadius;
    const Math::Vector3D dx(Math::Coords{EPSILON, 0, 0});
    const Math::Vector3D dy(Math::Coords{0, EPSILON, 0});
    const Math::Vector3D dz(Math::Coords{0, 0, EPSILON});
    double gx = fractalType->distanceEstimator(p + dx, center, maxIterations, bailout, power) -
                fractalType->distanceEstimator(p - dx, center, maxIterations, bailout, power);
    double gy = fractalType->distanceEstimator(p + dy, center, maxIterations, bailout, power) -
                fractalType->distanceEstimator(p - dy, center, maxIterations, bailout, power);
    double gz = fractalType->distanceEstimator(p + dz, center, maxIterations, bailout, power) -
                fractalType->distanceEstimator(p - dz, center, maxIterations, bailout, power);
    Math::Vector3D grad(Math::Coords{gx, gy, gz});
    if (grad.length() < 1e-8) {
        return (center - p).normalize();
    }
    return grad.normalize();
}

std::shared_ptr<Material> Fractal::getMaterial() const {
    return material;
}

std::shared_ptr<IPrimitive> Fractal::clone() const {
    auto copy = std::make_shared<Fractal>(center, boundingRadius, fractalType->getName(),
                                          maxIterations, bailout, material);
    copy->rotationX = rotationX;
    copy->rotationY = rotationY;
    copy->rotationZ = rotationZ;
    copy->power = power;
    copy->fractalType = fractalType->clone();
    copy->setSourceFile(sourceFile);
    return copy;
}

void Fractal::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    (*setting).add("x", libconfig::Setting::TypeFloat) = center.X;
    (*setting).add("y", libconfig::Setting::TypeFloat) = center.Y;
    (*setting).add("z", libconfig::Setting::TypeFloat) = center.Z;
    (*setting).add("boundingRadius", libconfig::Setting::TypeFloat) = boundingRadius;
    (*setting).add("fractalType", libconfig::Setting::TypeString) = fractalType->getName();
    (*setting).add("maxIterations", libconfig::Setting::TypeInt) = maxIterations;
    (*setting).add("bailout", libconfig::Setting::TypeFloat) = bailout;
    (*setting).add("power", libconfig::Setting::TypeFloat) = power;

    if (auto juliaFractal = std::dynamic_pointer_cast<JuliaFractal>(fractalType)) {
        Math::Point3D c = juliaFractal->getJuliaConstant();
        (*setting).add("julia_x", libconfig::Setting::TypeFloat) = c.X;
        (*setting).add("julia_y", libconfig::Setting::TypeFloat) = c.Y;
        (*setting).add("julia_z", libconfig::Setting::TypeFloat) = c.Z;
    } else if (auto quaternionFractal = std::dynamic_pointer_cast<QuaternionJuliaFractal>(fractalType)) {
        auto constant = quaternionFractal->getConstant();
        (*setting).add("qjulia_x", libconfig::Setting::TypeFloat) = constant.x;
        (*setting).add("qjulia_y", libconfig::Setting::TypeFloat) = constant.y;
        (*setting).add("qjulia_z", libconfig::Setting::TypeFloat) = constant.z;
        (*setting).add("qjulia_w", libconfig::Setting::TypeFloat) = constant.w;
    } else if (auto mengerFractal = std::dynamic_pointer_cast<MengerSpongeFractal>(fractalType)) {
        (*setting).add("menger_scale", libconfig::Setting::TypeFloat) = mengerFractal->getScale();
    } else if (auto sierpinskiFractal = std::dynamic_pointer_cast<SierpinskiTetrahedronFractal>(fractalType)) {
        (*setting).add("sierpinski_scale", libconfig::Setting::TypeFloat) = sierpinskiFractal->getScale();
    } else if (auto mandelboxFractal = std::dynamic_pointer_cast<MandelboxFractal>(fractalType)) {
        (*setting).add("mandelbox_scale", libconfig::Setting::TypeFloat) = mandelboxFractal->getScale();
        (*setting).add("mandelbox_minRadius", libconfig::Setting::TypeFloat) = mandelboxFractal->getMinRadius2();
        (*setting).add("mandelbox_foldingLimit", libconfig::Setting::TypeFloat) = mandelboxFractal->getFoldingLimit();
    }
}

}  // namespace RayTracer
