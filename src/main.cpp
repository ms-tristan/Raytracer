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
#include <fstream>

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
    bool ppmOutput = false;
    std::string ppmFilename;

    try {
        // Parse command line arguments
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (arg == "--ppm" && i + 1 < argc) {
                ppmOutput = true;
                ppmFilename = argv[i + 1];
                i++; // Skip the next argument as we've already processed it
            } else if (!ppmOutput) {
                // Only use the first non-flag argument as scene file
                sceneFile = arg;
            }
        }

        if (ppmOutput) {
            std::cout << "Rendering to PPM file: " << ppmFilename << std::endl;
        }

        std::cout << "Loading scene from file: " << sceneFile << std::endl;
        scene = director.createSceneFromFile(sceneFile);
        if (!scene) {
            std::cout << "Failed to load scene, using default scene instead." << std::endl;
            scene = director.createDefaultScene();
        }

        auto camera = std::make_shared<RayTracer::Camera>(scene->getCamera());

        if (ppmOutput) {
            // Output to PPM file
            std::ofstream outfile(ppmFilename);
            if (!outfile.is_open()) {
                std::cerr << "Failed to open output file: " << ppmFilename << std::endl;
                return 1;
            }

            // Write PPM header
            outfile << "P3" << std::endl;
            outfile << image_width << " " << image_height << std::endl;
            outfile << "255" << std::endl;

            // Render scene to PPM
            for (int j = image_height - 1; j >= 0; --j) {
                std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
                for (int i = 0; i < image_width; ++i) {
                    double u = static_cast<double>(i) / (image_width - 1);
                    double v = static_cast<double>(j) / (image_height - 1);

                    RayTracer::Ray ray = camera->ray(u, v);
                    Math::Vector3D pixelColor = scene->computeColor(ray);

                    // Write the color to the file
                    int r = static_cast<int>(255.999 * std::clamp(pixelColor.X, 0.0, 1.0));
                    int g = static_cast<int>(255.999 * std::clamp(pixelColor.Y, 0.0, 1.0));
                    int b = static_cast<int>(255.999 * std::clamp(pixelColor.Z, 0.0, 1.0));

                    outfile << r << ' ' << g << ' ' << b << std::endl;
                }
            }
            std::cout << "\nDone." << std::endl;
            outfile.close();
        } else {
            // Interactive SFML display
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
