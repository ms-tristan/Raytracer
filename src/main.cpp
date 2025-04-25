// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main with demonstration of all features including plugins and decorators
*/
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>
#include <thread>
#include <chrono>
#include <map>

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
#include "Primitive/PrimitiveDecorator/PrimitiveDecorator.hpp"
#include "Primitive/Plugin/PrimitivePluginManager.hpp"

// Lights and Materials
#include "Material/Material.hpp"
#include "Material/MaterialDecorator/MaterialDecorator.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/PointLight/PointLight.hpp"
#include "Light/LightDecorator/LightDecorator.hpp"

// Math utilities
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Ray/Ray.hpp"

// Observer Pattern for Scene File Reloading
#include "IObserver/SceneFileWatcher/SceneFileWatcher.hpp"
#include "IObserver/SceneFileWatcher/SceneReloader.hpp"

void write_color(const Math::Vector3D& color) {
    double r = std::sqrt(std::max(0.0, std::min(1.0, color.X)));
    double g = std::sqrt(std::max(0.0, std::min(1.0, color.Y)));
    double b = std::sqrt(std::max(0.0, std::min(1.0, color.Z)));

    int ir = static_cast<int>(255.999 * r);
    int ig = static_cast<int>(255.999 * g);
    int ib = static_cast<int>(255.999 * b);

    std::cout << ir << " " << ig << " " << ib << std::endl;
}

class RotatingDecorator : public RayTracer::PrimitiveDecorator {
public:
    RotatingDecorator(std::shared_ptr<RayTracer::IPrimitive> primitive, 
                     double degrees, const Math::Vector3D& axis) 
        : PrimitiveDecorator(primitive), degrees(degrees), axis(axis), currentAngle(0) {}

    std::optional<RayTracer::HitInfo> hit(const RayTracer::Ray &ray, 
                                         double tMin, double tMax) const override {
        // Apply current rotation then hit test
        auto result = wrappedPrimitive->hit(ray, tMin, tMax);
        return result;
    }

    void update() {
        // Update rotation angle
        currentAngle += degrees;
        if (currentAngle >= 360.0) {
            currentAngle -= 360.0;
        }
        
        // Apply rotation based on axis
        if (axis.X > 0.5) wrappedPrimitive->rotateX(degrees);
        else if (axis.Y > 0.5) wrappedPrimitive->rotateY(degrees);
        else wrappedPrimitive->rotateZ(degrees);
    }

    std::shared_ptr<RayTracer::IPrimitive> clone() const override {
        return std::make_shared<RotatingDecorator>(
            wrappedPrimitive->clone(), degrees, axis);
    }

private:
    double degrees;
    Math::Vector3D axis;
    double currentAngle;
};

int main() {
    const int image_width = 800;
    const int image_height = 600;

    std::cout << "P3" << std::endl;
    std::cout << image_width << " " << image_height << std::endl;
    std::cout << "255" << std::endl;

    // Create materials
    auto redMaterial = std::make_shared<RayTracer::Material>();
    redMaterial->color = Math::Vector3D(Math::Coords{0.9, 0.2, 0.2});
    // Reflection property removed

    auto blueMaterial = std::make_shared<RayTracer::Material>();
    blueMaterial->color = Math::Vector3D(Math::Coords{0.0, 0.1, 0.9});
    // Reflection property removed

    auto greenMaterial = std::make_shared<RayTracer::Material>();
    greenMaterial->color = Math::Vector3D(Math::Coords{0.1, 0.8, 0.2});
    // Reflection property removed

    auto planeMaterial = std::make_shared<RayTracer::Material>();
    planeMaterial->color = Math::Vector3D(Math::Coords{0.396, 0.502, 0.651});
    // Reflection property removed

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
        0.032  // Quadratic attenuation
    );
    builder.addLight(pointLight);
    
    // Add a red sphere directly
    builder.addSphere(
        Math::Point3D(Math::Coords{-0.9, 0.5, -1.3}),
        0.4,
        redMaterial
    );
    
    // PLUGIN SYSTEM: Load primitive plugins
    auto pluginManager = RayTracer::PrimitivePluginManager::getInstance();
    
    // Load box plugin
    if (pluginManager->loadPlugin("./plugins/box_plugin.so")) {
        
        // Create parameters for the box
        std::map<std::string, double> boxParams = {
            {"x", 0.5}, {"y", 0.2}, {"z", -1.2},  // Position
            {"width", 0.4}, {"height", 0.3}, {"depth", 0.4}  // Dimensions
        };
        
        // Create box using plugin
        auto boxPrimitive = pluginManager->createPrimitive(
            "box", boxParams, greenMaterial);
            
        if (boxPrimitive) {
            builder.addPrimitive(boxPrimitive);
        }
    } else {
        // Fallback to cylinder if plugin fails
        auto cylinder = std::make_shared<RayTracer::Cylinder>(
            Math::Point3D(Math::Coords{0.8, -0.3, -1.1}),
            Math::Vector3D(Math::Coords{0, 1, 0}),
            0.3, 0.8, greenMaterial);
        cylinder->rotateZ(30);
        builder.addPrimitive(cylinder);
    }
    
    // DECORATOR PATTERN: Use PrimitiveDecorator for special effects
    // Create a base cone
    auto cone = std::make_shared<RayTracer::Cone>(
        Math::Point3D(Math::Coords{-0.4, 0.3, -1.5}),
        Math::Vector3D(Math::Coords{0, -1, 0}),
        0.4, 0.8, blueMaterial);
    
    // Wrap it in a rotating decorator
    auto rotatingCone = std::make_shared<RotatingDecorator>(
        cone, 1.0, Math::Vector3D(Math::Coords{0, 0, 1}));
    
    // COMPOSITE PATTERN: Group objects together
    auto compositeMaterial = std::make_shared<RayTracer::Material>();
    compositeMaterial->color = Math::Vector3D(Math::Coords{0.5, 0.5, 0.5}); // Gray base
    auto compositeGroup = std::make_shared<RayTracer::CompositePrimitive>(compositeMaterial);
    
    // Add the decorated cone to the composite group
    compositeGroup->add(rotatingCone);
    
    // Add a small sphere near the cone in the same group
    auto smallSphere = std::make_shared<RayTracer::Sphere>(
        Math::Point3D(Math::Coords{-0.1, -0.1, -1.0}),
        0.15,
        redMaterial);
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
    
    // OBSERVER PATTERN: This section is commented out since the implementation is missing
    /*
    std::cout << "# Setting up SceneFileWatcher (demonstration)" << std::endl;
    RayTracer::SceneFileWatcher fileWatcher("scene.config");
    auto sceneReloader = std::make_shared<RayTracer::SceneReloader>("scene.config");
    fileWatcher.attach(sceneReloader);
    */


    // Animation loop (for demonstration)
    int frameCount = 1; // Just render one frame for this example
    for (int frame = 0; frame < frameCount; frame++) {
        // Update any animated primitives
        rotatingCone->update();
        
        // Check if scene file has changed (would trigger reload)
        // fileWatcher.checkForChanges();
        
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
        
        // In a real animation, would wait for next frame here
        // std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
    }
    
    return 0;
}
