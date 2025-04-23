// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main
*/
#include <iostream>
#include <memory>
#include <algorithm>
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder/SceneBuilder.hpp"
#include "Scene/SceneDirector/SceneDirector.hpp"
#include "Camera/Camera.hpp"
#include "Rectangle3D/Rectangle3D.hpp"
#include "Material/Material.hpp"
#include "Primitive/Sphere/Sphere.hpp"
#include "Primitive/Cylinder/Cylinder.hpp"
#include "Primitive/Cone/Cone.hpp"
#include "Primitive/Plane/Plane.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Ray/Ray.hpp"

void write_color(const Math::Vector3D& color) {
    double r = std::sqrt(std::max(0.0, std::min(1.0, color.X)));
    double g = std::sqrt(std::max(0.0, std::min(1.0, color.Y)));
    double b = std::sqrt(std::max(0.0, std::min(1.0, color.Z)));

    int ir = static_cast<int>(255.999 * r);
    int ig = static_cast<int>(255.999 * g);
    int ib = static_cast<int>(255.999 * b);

    std::cout << ir << " " << ig << " " << ib << std::endl;
}

int main() {
    const int image_width = 800;
    const int image_height = 600;

    std::cout << "P3" << std::endl;
    std::cout << image_width << " " << image_height << std::endl;
    std::cout << "255" << std::endl;

    // Let's use the SceneDirector to create a default scene and modify it
    RayTracer::SceneDirector director;
    std::unique_ptr<RayTracer::Scene> scene = director.createDefaultScene();
    
    // Create our own scene instead
    RayTracer::SceneBuilder builder;
    
    // Create materials
    auto redMaterial = std::make_shared<RayTracer::Material>();
    redMaterial->color = Math::Vector3D(Math::Coords{0.9, 0.2, 0.2});
    
    auto blueMaterial = std::make_shared<RayTracer::Material>();
    blueMaterial->color = Math::Vector3D(Math::Coords{0.0, 0.1, 0.9}); // Deep blue with less green influence
    
    auto greenMaterial = std::make_shared<RayTracer::Material>();
    greenMaterial->color = Math::Vector3D(Math::Coords{0.1, 0.8, 0.2});
    
    auto planeMaterial = std::make_shared<RayTracer::Material>();
    planeMaterial->color = Math::Vector3D(Math::Coords{0.396, 0.502, 0.651});

    // First, set up the camera
    builder.setCamera(
        Math::Point3D(Math::Coords{0.0, 1.0, 2.5}),
        Math::Point3D(Math::Coords{0.0, 0.0, -1.0})
    );
    
    // Set ambient light - increase slightly to reduce harsh shadows
    builder.setAmbientLight(Math::Vector3D(Math::Coords{0.15, 0.15, 0.18}));
    
    // Add directional lights - adjust direction slightly
    auto mainLight = std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3D(Math::Coords{0.3, -1.0, -0.5}),  // Changed direction slightly
        Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}));
    builder.addLight(mainLight);
    
    auto fillLight = std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3D(Math::Coords{-0.6, -0.4, -0.2}),  // Adjusted fill light
        Math::Vector3D(Math::Coords{0.5, 0.5, 0.6}));    // Slightly brighter
    builder.addLight(fillLight);
    
    // Add primitives - adjust positions to minimize shadow interactions
    builder.addSphere(Math::Point3D(Math::Coords{-0.9, 0.5, -1.3}), 0.4, redMaterial);  // Moved back and higher
    
    auto cylinder = std::make_shared<RayTracer::Cylinder>(
        Math::Point3D(Math::Coords{0.8, -0.3, -1.1}),  // Moved right
        Math::Vector3D(Math::Coords{0, 1, 0}),
        0.3,
        0.8,
        greenMaterial);
    cylinder->rotateZ(30);
    builder.addPrimitive(cylinder);
    
    auto cone = std::make_shared<RayTracer::Cone>(
        Math::Point3D(Math::Coords{-0.4, 0.3, -1.5}),  // Moved to the left (negative X)
        Math::Vector3D(Math::Coords{0, -1, 0}),
        0.4,
        0.8,
        blueMaterial);
    // cone->rotateX(15);
    builder.addPrimitive(cone);
    
    // Lower the ground plane slightly to reduce shadow interaction
    builder.addPlane(
        Math::Point3D(Math::Coords{0, -0.7, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        planeMaterial);
    
    // Build the scene
    scene = builder.build();
    
    // Render the scene
    RayTracer::Camera camera = scene->getCamera();
    
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            double u = static_cast<double>(i) / (image_width - 1);
            double v = static_cast<double>(j) / (image_height - 1);

            RayTracer::Ray ray = camera.ray(u, v);
            Math::Vector3D pixelColor = scene->computeColor(ray);

            write_color(pixelColor);
        }
    }
    return 0;
}
