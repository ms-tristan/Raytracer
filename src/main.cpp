// // Copyright <2025> Epitech
// /*
// ** EPITECH PROJECT, 2025
// ** Raytracer
// ** File description:
// ** Main
// */
// #include <iostream>
// #include <memory>
// #include <algorithm>
// #include "Scene/Scene.hpp"
// #include "Scene/SceneBuilder/SceneBuilder.hpp"
// #include "Scene/SceneDirector/SceneDirector.hpp"
// #include "Camera/Camera.hpp"
// #include "Rectangle3D/Rectangle3D.hpp"
// #include "Material/Material.hpp"
// #include "Primitive/Sphere/Sphere.hpp"
// #include "Primitive/Cylinder/Cylinder.hpp"
// #include "Primitive/Cone/Cone.hpp"
// #include "Primitive/Plane/Plane.hpp"
// #include "Light/DirectionalLight/DirectionalLight.hpp"
// #include "Light/AmbientLight/AmbientLight.hpp"
// #include "Math/Vector3D/Vector3D.hpp"
// #include "Math/Point3D/Point3D.hpp"
// #include "Ray/Ray.hpp"

// void write_color(const Math::Vector3D& color) {
//     double r = std::sqrt(std::max(0.0, std::min(1.0, color.X)));
//     double g = std::sqrt(std::max(0.0, std::min(1.0, color.Y)));
//     double b = std::sqrt(std::max(0.0, std::min(1.0, color.Z)));

//     int ir = static_cast<int>(255.999 * r);
//     int ig = static_cast<int>(255.999 * g);
//     int ib = static_cast<int>(255.999 * b);

//     std::cout << ir << " " << ig << " " << ib << std::endl;
// }

// int main() {
//     const int image_width = 800;
//     const int image_height = 600;

//     std::cout << "P3" << std::endl;
//     std::cout << image_width << " " << image_height << std::endl;
//     std::cout << "255" << std::endl;

//     RayTracer::SceneDirector director;
//     std::unique_ptr<RayTracer::Scene> scene = director.createDefaultScene();

//     RayTracer::SceneBuilder builder;

//     auto redMaterial = std::make_shared<RayTracer::Material>();
//     redMaterial->color = Math::Vector3D(Math::Coords{0.9, 0.2, 0.2});

//     auto blueMaterial = std::make_shared<RayTracer::Material>();
//     blueMaterial->color = Math::Vector3D(Math::Coords{0.0, 0.1, 0.9});

//     auto greenMaterial = std::make_shared<RayTracer::Material>();
//     greenMaterial->color = Math::Vector3D(Math::Coords{0.1, 0.8, 0.2});

//     auto planeMaterial = std::make_shared<RayTracer::Material>();
//     planeMaterial->color = Math::Vector3D(Math::Coords{0.396, 0.502, 0.651});

//     builder.setCamera(
//         Math::Point3D(Math::Coords{0.0, 1.0, 2.5}),
//         Math::Point3D(Math::Coords{0.0, 0.0, -1.0}));

//     builder.setAmbientLight(Math::Vector3D(Math::Coords{0.15, 0.15, 0.18}));

//     auto mainLight = std::make_shared<RayTracer::DirectionalLight>(
//         Math::Vector3D(Math::Coords{0.3, -1.0, -0.5}),
//         Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}));
//     builder.addLight(mainLight);

//     auto fillLight = std::make_shared<RayTracer::DirectionalLight>(
//         Math::Vector3D(Math::Coords{-0.6, -0.4, -0.2}),
//         Math::Vector3D(Math::Coords{0.5, 0.5, 0.6}));
//     builder.addLight(fillLight);

//     builder.addSphere(Math::Point3D(Math::Coords{-0.9, 0.5, -1.3}),
//         0.4, redMaterial);

//     auto cylinder = std::make_shared<RayTracer::Cylinder>(
//         Math::Point3D(Math::Coords{0.8, -0.3, -1.1}),
//         Math::Vector3D(Math::Coords{0, 1, 0}),
//         0.3,
//         0.8,
//         greenMaterial);
//     cylinder->rotateZ(30);
//     builder.addPrimitive(cylinder);

//     auto cone = std::make_shared<RayTracer::Cone>(
//         Math::Point3D(Math::Coords{-0.4, 0.3, -1.5}),
//         Math::Vector3D(Math::Coords{0, -1, 0}),
//         0.4,
//         0.8,
//         blueMaterial);
//     cone->rotateX(15);
//     builder.addPrimitive(cone);

//     builder.addPlane(
//         Math::Point3D(Math::Coords{0, -0.7, 0}),
//         Math::Vector3D(Math::Coords{0, 1, 0}),
//         planeMaterial);

//     scene = builder.build();

//     RayTracer::Camera camera = scene->getCamera();

//     for (int j = image_height - 1; j >= 0; --j) {
//         for (int i = 0; i < image_width; ++i) {
//             double u = static_cast<double>(i) / (image_width - 1);
//             double v = static_cast<double>(j) / (image_height - 1);

//             RayTracer::Ray ray = camera.ray(u, v);
//             Math::Vector3D pixelColor = scene->computeColor(ray);

//             write_color(pixelColor);
//         }
//     }
//     return 0;
// }

// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main with demonstration of design patterns
*/
#include <iostream>
#include <memory>
#include <algorithm>

// Scene components
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder/SceneBuilder.hpp"
#include "Scene/SceneDirector/SceneDirector.hpp"
#include "Camera/Camera.hpp"
#include "Rectangle3D/Rectangle3D.hpp"

// Primitives
#include "Primitive/Sphere/Sphere.hpp"
#include "Primitive/Cylinder/Cylinder.hpp"
#include "Primitive/Cone/Cone.hpp"
#include "Primitive/Plane/Plane.hpp"
#include "Primitive/CompositePrimitive/CompositePrimitive.hpp"

// Lights and Materials
#include "Material/Material.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/PointLight/PointLight.hpp"

// Math utilities
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

    // Create materials
    auto redMaterial = std::make_shared<RayTracer::Material>();
    redMaterial->color = Math::Vector3D(Math::Coords{0.9, 0.2, 0.2});

    auto blueMaterial = std::make_shared<RayTracer::Material>();
    blueMaterial->color = Math::Vector3D(Math::Coords{0.0, 0.1, 0.9});

    auto greenMaterial = std::make_shared<RayTracer::Material>();
    greenMaterial->color = Math::Vector3D(Math::Coords{0.1, 0.8, 0.2});

    auto planeMaterial = std::make_shared<RayTracer::Material>();
    planeMaterial->color = Math::Vector3D(Math::Coords{0.396, 0.502, 0.651});

    // BUILDER PATTERN: Use SceneBuilder to construct the scene
    RayTracer::SceneBuilder builder;

    builder.setCamera(
        Math::Point3D(Math::Coords{0.0, 1.0, 2.5}),
        Math::Point3D(Math::Coords{0.0, 0.0, -1.0}));

    builder.setAmbientLight(Math::Vector3D(Math::Coords{0.15, 0.15, 0.18}));

    auto mainLight = std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3D(Math::Coords{0.3, -1.0, -0.5}),
        Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}));
    builder.addLight(mainLight);

    auto fillLight = std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3D(Math::Coords{-0.6, -0.4, -0.2}),
        Math::Vector3D(Math::Coords{0.5, 0.5, 0.6}));
    builder.addLight(fillLight);

    auto pointLight = std::make_shared<RayTracer::PointLight>(
        Math::Point3D(Math::Coords{1.0, 1.0, 0.0}),  // Position
        Math::Vector3D(Math::Coords{0.8, 0.8, 1.0}),  // Color (slight blue tint)
        1.0,   // Constant attenuation
        0.09,  // Linear attenuation
        0.032
    );
    builder.addLight(pointLight);
    
    // Add a red sphere directly
    builder.addSphere(
        Math::Point3D(Math::Coords{-0.9, 0.5, -1.3}),
        0.4,
        redMaterial
    );
    
    // Create a cylinder manually
    auto cylinder = std::make_shared<RayTracer::Cylinder>(
        Math::Point3D(Math::Coords{0.8, -0.3, -1.1}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        0.3,
        0.8,
        greenMaterial);
    cylinder->rotateZ(30);
    builder.addPrimitive(cylinder);

    // COMPOSITE PATTERN: Group objects together
    auto compositeMaterial = std::make_shared<RayTracer::Material>();
    compositeMaterial->color = Math::Vector3D(Math::Coords{0.5, 0.5, 0.5}); // Gray material for group
    auto compositeGroup = std::make_shared<RayTracer::CompositePrimitive>(compositeMaterial);
    
    // Add a cone to the composite group
    auto cone = std::make_shared<RayTracer::Cone>(
        Math::Point3D(Math::Coords{-0.4, 0.3, -1.5}),
        Math::Vector3D(Math::Coords{0, -1, 0}),
        0.4,
        0.8,
        blueMaterial);
    cone->rotateX(15);
    compositeGroup->add(cone);
    
    // Add a small sphere near the cone in the same group
    auto smallSphere = std::make_shared<RayTracer::Sphere>(
        Math::Point3D(Math::Coords{-0.1, -0.1, -1.0}),
        0.15,
        redMaterial);  // Changed to red material for contrast
    compositeGroup->add(smallSphere);
    
    // Add the entire composite group to the scene
    builder.addPrimitive(compositeGroup);

    // Add a floor plane
    builder.addPlane(
        Math::Point3D(Math::Coords{0, -0.7, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        planeMaterial);

    // Build the scene
    std::unique_ptr<RayTracer::Scene> scene = builder.build();
    
    // Get the camera from the scene
    RayTracer::Camera camera = scene->getCamera();
    
    // Comment about missing features that would be useful
    std::cout << "# Rendering scene..." << std::endl;
    std::cout << "# Note: Additional features like SceneDirector presets, " << std::endl;
    std::cout << "# PrimitiveDecorators, and SceneFileWatcher could enhance this raytracer." << std::endl;

    // Render the scene
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
