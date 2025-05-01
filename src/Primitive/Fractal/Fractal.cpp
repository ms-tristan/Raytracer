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
#include <limits> // Added for std::numeric_limits
#include "Primitive/Fractal/Fractal.hpp"
#include "Primitive/Fractal/FractalType/Julia/JuliaFractal.hpp"
#include "Primitive/Fractal/FractalType/Mandelbox/MandelboxFractal.hpp"
#include "Primitive/Fractal/FractalType/MengerSponge/MengerSpongeFractal.hpp"
#include "Primitive/Fractal/FractalType/Sierpinski/SierpinskiTetrahedronFractal.hpp"
#include "Primitive/Fractal/FractalType/QuaternionJulia/QuaternionJuliaFractal.hpp"

namespace RayTracer {

// Constructors
Fractal::Fractal(const Math::Point3D &center, double boundingRadius,
                 const std::string &fractalTypeName, int maxIterations, double bailout)
    : material(std::make_shared<Material>()), center(center),
      boundingRadius(boundingRadius), maxIterations(maxIterations), 
      bailout(bailout), power(2.0) {
    // Create the fractal type from the factory
    fractalType = FractalTypeFactory::getInstance().createFractalType(fractalTypeName);
}

Fractal::Fractal(const Math::Point3D &center, double boundingRadius,
                 const std::string &fractalTypeName, int maxIterations, double bailout,
                 const std::shared_ptr<Material> &material)
    : material(material), center(center), boundingRadius(boundingRadius),
      maxIterations(maxIterations), bailout(bailout), power(2.0) {
    // Create the fractal type from the factory
    fractalType = FractalTypeFactory::getInstance().createFractalType(fractalTypeName);
}

// Fractal type and parameter methods
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

// Type-specific parameter setters
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
        // Note: SierpinskiTetrahedronFractal doesn't have useTetrahedron parameter anymore
        // as we've created a specific implementation for tetrahedron
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

// Transformation methods
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

// Main ray intersection method
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

    // First check if the ray intersects the bounding sphere
    Math::Vector3D oc = transformedRay.origin - center;
    double a = transformedRay.direction.dot(transformedRay.direction);
    double b = 2.0 * oc.dot(transformedRay.direction);
    double c = oc.dot(oc) - boundingRadius * boundingRadius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return std::nullopt;  // Ray doesn't intersect the bounding sphere

    // If we've passed the bounding sphere test, perform ray marching
    return rayMarch(transformedRay, tMin, tMax);
}

// Ray marching implementation
std::optional<HitInfo> Fractal::rayMarch(const Ray& ray, double tMin, double tMax) const {
    const double PRECISION = 0.0001;  // Precision for surface detection
    const int MAX_STEPS = 300;        // Increased maximum marching steps for better results
    
    // Surface threshold based on bounding radius
    const double EPSILON = 0.001 * (boundingRadius / 10.0);
    
    double t = tMin;
    double lastDistance = std::numeric_limits<double>::max();
    int stepsSinceReduction = 0;
    
    std::cout << "Starting ray marching for fractal: " << fractalType->getName() << std::endl;
    std::cout << "Params: MAX_STEPS=" << MAX_STEPS << ", EPSILON=" << EPSILON 
        << ", tMin=" << tMin << ", tMax=" << (tMax == std::numeric_limits<double>::infinity() ? "inf" : std::to_string(tMax)) << std::endl;
    
    // Ray marching loop
    for (int i = 0; i < MAX_STEPS && t < tMax; i++) {
        Math::Point3D point = ray.origin + ray.direction * t;
        
        // Get the distance to the fractal
        double distance = fractalType->distanceEstimator(point, center, maxIterations, bailout, power);
        
        // Improve numerical stability - clamp very small values
        distance = std::max(distance, 1e-10);
        
        // Adaptive step size for better detail resolution
        double stepFactor = 0.5;  // Conservative factor - start with small steps
        
        // If we've made progress without issues, gradually increase step size
        if (stepsSinceReduction > 5) {
            stepFactor = std::min(0.8, stepFactor * 1.1);
        }
        
        // If we're close to the surface, use very small steps for precision
        if (distance < EPSILON * 5.0) {
            stepFactor = 0.1;
            stepsSinceReduction = 0;
        }
        
        // Surface detection logic - we've hit the fractal surface
        if (distance < EPSILON) {
            // Uncomment for detailed debugging
            // std::cout << "Surface hit at distance: " << distance << " (threshold: " << EPSILON << ")" << std::endl;
            // std::cout << "Steps taken: " << i + 1 << " of " << MAX_STEPS << std::endl;
            
            HitInfo info;
            info.distance = t;
            info.hitPoint = ray.origin + ray.direction * t;
            
            // Calculate normal using the central differences method
            info.normal = estimateNormal(info.hitPoint);
            
            // Normalize the normal vector to ensure a unit vector
            info.normal = info.normal.normalize();
            
            info.primitive = const_cast<Fractal*>(this)->shared_from_this();
            
            // Apply rotation to normal if needed
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
                
                // Ensure normal is still normalized after rotations
                info.normal = info.normal.normalize();
            }
            
            // Comment out debug output that uses X, Y, Z to avoid compilation errors
            // std::cout << "Hit normal: (" << info.normal.X << ", " 
            //           << info.normal.Y << ", " << info.normal.Z << ")" << std::endl;
                      
            return info;
        }
        
        // If we're getting closer, that's good
        if (distance < lastDistance) {
            stepsSinceReduction++;
        } else {
            // If we're oscillating or moving away, take smaller steps
            stepFactor *= 0.5;
            stepsSinceReduction = 0;
        }
        
        lastDistance = distance;
        
        // Take the next step (adaptive)
        double step = distance * stepFactor;
        
        // Ensure we don't take excessively small steps that could cause getting stuck
        if (step < PRECISION) {
            step = PRECISION;
        }
        
        t += step;
    }
    
    // No hit found
    return std::nullopt;
}

// Normal estimation using central differences of the distance field
Math::Vector3D Fractal::estimateNormal(const Math::Point3D& p) const {
    // Use a small epsilon value scaled to the bounding radius
    const double EPSILON = 0.0001 * boundingRadius;
    const Math::Vector3D dx(Math::Coords{EPSILON, 0, 0});
    const Math::Vector3D dy(Math::Coords{0, EPSILON, 0});
    const Math::Vector3D dz(Math::Coords{0, 0, EPSILON});
    
    // Calculate gradient using central differences
    double gx = fractalType->distanceEstimator(p + dx, center, maxIterations, bailout, power) - 
                fractalType->distanceEstimator(p - dx, center, maxIterations, bailout, power);
    double gy = fractalType->distanceEstimator(p + dy, center, maxIterations, bailout, power) - 
                fractalType->distanceEstimator(p - dy, center, maxIterations, bailout, power);
    double gz = fractalType->distanceEstimator(p + dz, center, maxIterations, bailout, power) - 
                fractalType->distanceEstimator(p - dz, center, maxIterations, bailout, power);
    
    // Create and normalize the gradient vector
    return Math::Vector3D(Math::Coords{gx, gy, gz}).normalize();
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
    
    // Add specific parameters for different fractal types
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