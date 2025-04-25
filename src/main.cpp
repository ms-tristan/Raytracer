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
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include <SFML/Graphics.hpp>

// Display Manager
#include "./Renderer/DisplayManager/SFMLDisplayManager.hpp"
#include "./Renderer/Renderer.hpp"

// Events Manager
#include "./EventsManager/SFMLEventsManager.hpp"

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


int main(int argc, char **argv) {
    const int image_width = 800;
    const int image_height = 600;

    RayTracer::SceneDirector director;
    std::unique_ptr<RayTracer::Scene> scene;

    if (argc > 1) {
        std::string sceneFile = argv[1];
        std::cout << "Loading scene from file: " << sceneFile << std::endl;
        scene = director.createSceneFromFile(sceneFile);
    } else {
        // Create a hardcoded scene if no file was specified
        std::cout << "No scene file specified, using default scene." << std::endl;
        scene = director.createDefaultScene();
    }

    RayTracer::Camera camera = scene->getCamera();

    RayTracer::SFMLDisplayManager displayManager;

    RayTracer::SFMLEventsManager eventsManager(displayManager.getWindow());

    displayManager.initialize(image_width, image_height, "Raytracer", false);

    RayTracer::Renderer renderer(displayManager);

    while (displayManager.isWindowOpen()) {

        renderer.drawScene(*scene, camera);
        eventsManager.processEvents();
        if (eventsManager.isKeyPressed("SPACE")) {
            camera.rotateX(10);
        }

    }

    return 0;
}
