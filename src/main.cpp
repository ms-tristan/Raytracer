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

#include <thread>
#include <vector>
#include <mutex>

#include <SFML/Graphics.hpp>

// Display Manager
#include "./Renderer/DisplayManager/SFMLDisplayManager.hpp"
#include "./Renderer/Renderer.hpp"

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


    RayTracer::SFMLDisplayManager displayManager;
    displayManager.initialize(image_width, image_height, "Raytracer", false);

    RayTracer::Renderer renderer(displayManager);

    while (displayManager.isWindowOpen()) {

        renderer.drawScene(*scene, camera);

        sf::Event event;
        while (displayManager.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                displayManager.closeWindow();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)
                    camera.rotateY(-5);
                if (event.key.code == sf::Keyboard::Right)
                    camera.rotateY(5);
                if (event.key.code == sf::Keyboard::Up)
                    camera.rotateX(-5);
                if (event.key.code == sf::Keyboard::Down)
                    camera.rotateX(5);
                if (event.key.code == sf::Keyboard::Space) {
                    camera.origin = Math::Point3D(Math::Coords{
                        camera.origin.X, camera.origin.Y + 0.1, camera.origin.Z
                    });
                }
                if (event.key.code == sf::Keyboard::LControl) {
                    camera.origin = Math::Point3D(Math::Coords{
                        camera.origin.X, camera.origin.Y - 0.1, camera.origin.Z
                    });
                }
            }
        }
    }

    return 0;
}
