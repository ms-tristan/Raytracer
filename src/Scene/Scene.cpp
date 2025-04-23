// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scene implementation
*/
#include "Scene/Scene.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>

namespace RayTracer {

void Scene::setCamera(const Camera &cam) { camera = cam; }

Camera Scene::getCamera() const { return camera; }

void Scene::setAmbientLight(const AmbientLight &light) { ambientLight = light; }

void Scene::addPrimitive(const std::shared_ptr<IPrimitive> &primitive) {
    primitives.push_back(primitive);
}

void Scene::addLight(const std::shared_ptr<ILight> &light) { lights.push_back(light); }

std::optional<HitInfo> Scene::trace(const Ray &ray) const {
    double closest = std::numeric_limits<double>::infinity();
    std::optional<HitInfo> closestHit;

    for (const auto &primitive : primitives) {
        auto hit = primitive->hit(ray, 0.001, closest);
        if (hit) {
            closest = hit->distance;
            closestHit = hit;
        }
    }

    return closestHit;
}

bool Scene::isInShadow(const Math::Point3D &hitPoint, const Math::Vector3D &lightDir) const {
    double shadowBias = 0.001;
    // Use the light direction directly for the shadow bias offset
    Math::Point3D shadowOrigin = hitPoint + lightDir * shadowBias;
    Ray shadowRay(shadowOrigin, lightDir);
    
    for (const auto &primitive : primitives) {
        // Use hit() to check for intersection along the shadow ray
        auto shadowHit = primitive->hit(shadowRay, 0.001, std::numeric_limits<double>::infinity());
        if (shadowHit) {
            return true;
        }
    }
    return false;
}

Math::Vector3D Scene::computeColor(const Ray &ray) const {
    auto hit = trace(ray);
    if (!hit)
        return Math::Vector3D(Math::Coords{0.0, 0.0, 0.0});

    // Start with ambient lighting
    Math::Vector3D pixelColor = hit->primitive->getMaterial()->color * ambientLight.color;

    for (const auto &light : lights) {
        Math::Vector3D lightDir = light->getLightDirection(hit->hitPoint);
        
        // Calculate if point is in shadow
        bool shadowed = isInShadow(hit->hitPoint, lightDir);

        // Even if in shadow, add a small amount of diffuse lighting to soften shadows
        double diffuseFactor = std::max(0.0, hit->normal.dot(lightDir));
        
        if (shadowed) {
            // Add a small amount of diffuse light even in shadow (softer shadows)
            diffuseFactor *= 0.15; // 15% of normal diffuse lighting for shadowed areas
        }

        Math::Vector3D diffuse =
            hit->primitive->getMaterial()->color * light->getLightColor() * diffuseFactor;
        pixelColor += diffuse;
    }

    // Clamp final color values
    pixelColor.X = std::min(1.0, pixelColor.X);
    pixelColor.Y = std::min(1.0, pixelColor.Y);
    pixelColor.Z = std::min(1.0, pixelColor.Z);

    return pixelColor;
}

void Scene::writeColor(const Math::Vector3D &color) {
    double r = std::sqrt(std::max(0.0, std::min(1.0, color.X)));
    double g = std::sqrt(std::max(0.0, std::min(1.0, color.Y)));
    double b = std::sqrt(std::max(0.0, std::min(1.0, color.Z)));

    int ir = static_cast<int>(255.999 * r);
    int ig = static_cast<int>(255.999 * g);
    int ib = static_cast<int>(255.999 * b);

    std::cout << ir << " " << ig << " " << ib << std::endl;
}

} // namespace RayTracer
