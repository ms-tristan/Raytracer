
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
#include "./EventsManager/InputManager/InputManager.hpp"

// Scene components
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder/SceneBuilder.hpp"
#include "Scene/SceneDirector/SceneDirector.hpp"
#include "Camera/Camera.hpp"
#include "Rectangle3D/Rectangle3D.hpp"

// Primitives
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
    std::shared_ptr<RayTracer::Scene> scene;
    std::string sceneFile = "scenes/default_scene.cfg";

    try {
        if (argc > 1) {
            sceneFile = argv[1];
            std::cout << "Loading scene from file: " << sceneFile << std::endl;
            scene = director.createSceneFromFile(sceneFile);
        } else {
            std::cout << "No scene file specified, using default scene." << std::endl;
            scene = director.createDefaultScene();
        }

        auto camera = std::make_shared<RayTracer::Camera>(scene->getCamera());

        auto displayManager = std::make_shared<RayTracer::SFMLDisplayManager>();
        displayManager->initialize(image_width, image_height, "Raytracer", false);

        auto eventsManager = std::make_shared<RayTracer::SFMLEventsManager>(displayManager->getWindow());

        RayTracer::Renderer renderer(displayManager);

        RayTracer::InputManager inputManager(eventsManager, image_width, image_height);

        auto prims = scene->getPrimitives();

        while (displayManager->isWindowOpen()) {
            renderer.drawScene(*scene, *camera);
            inputManager.processInput(scene, camera);

            if (eventsManager->isKeyPressed("ESCAPE")) {
                displayManager->closeWindow();
            }
        }

        scene->setCamera(*camera);

        if (!director.saveSceneToFile(*scene, sceneFile)) {
            std::cerr << "Failed to save the scene to " << sceneFile << std::endl;
        } else {
            std::cout << "Scene saved successfully to " << sceneFile << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
}
