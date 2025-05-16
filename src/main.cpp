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
#include "WindowSize.hpp"
// Exception system
#include "Exception/IException.hpp"
#include "Exception/BaseException.hpp"
#include "Exception/ValueRangeException.hpp"
#include "Exception/PrimitiveNotFoundException.hpp"
#include "Exception/SceneImportException.hpp"
#include "Exception/FileIOException.hpp"
#include "Exception/ConfigParseException.hpp"
#include "Exception/InvalidOperationException.hpp"

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


void renderToPPM(const RayTracer::Scene& scene, const RayTracer::Camera& camera,
                 int width, int height, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw RayTracer::FileIOException(filename, "open", "Could not open file for writing");
    }

    outFile << "P3\n" << width << " " << height << "\n255\n";

    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(outFile.rdbuf());

    int samplesPerPixel = 1;
    for (const auto& postProcess : scene.getPostProcessEffects()) {
        if (postProcess->getTypeName() == "supersampling") {
            samplesPerPixel = static_cast<int>(postProcess->getParameter("samples"));
            break;
        }
    }

    const_cast<RayTracer::Scene&>(scene).setImageDimensions(width, height);
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            double u = static_cast<double>(i) / (width - 1);
            double v = static_cast<double>(j) / (height - 1);
            Math::Vector3D pixelColor;
            if (samplesPerPixel > 1) {
                pixelColor = camera.supersampleRay(u, v, scene, samplesPerPixel);
            } else {
                RayTracer::Ray ray = camera.ray(u, v);
                pixelColor = scene.computeColor(ray, false);
            }
            scene.writeColor(pixelColor);
        }
    }
    std::cout.rdbuf(oldCoutStreamBuf);
    outFile.close();
}

int main(int argc, char **argv) {
    const int image_width = WIDTH;
    const int image_height = HEIGHT;
    bool displayMode = true;
    std::string outputFile = "output.ppm";

    RayTracer::SceneDirector director;
    std::shared_ptr<RayTracer::Scene> scene;
    std::string sceneFile = "scenes/default_scene.cfg";
    bool ppmOutput = false;
    std::string ppmFilename;

    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--file" && i + 1 < argc) {
                sceneFile = argv[++i];
            } else if (arg == "--output" && i + 1 < argc) {
                outputFile = argv[++i];
            } else if (arg == "--no-display") {
                displayMode = false;
            } else if (arg == "--help") {
                std::cout << "Usage: ./raytracer [options]\n"
                          << "Options:\n"
                          << "  --file <filename>    Specify scene file (default: scenes/default_scene.cfg)\n"
                          << "  --output <filename>  Specify output PPM file (default: output.ppm)\n"
                          << "  --no-display         Render to PPM file without displaying window\n"
                          << "  --help               Display this help message\n";
                return 0;
            } else if (i == 1 && arg[0] != '-') {
                sceneFile = arg;
            }
        }

        scene = director.createSceneFromFile(sceneFile);

        if (!scene)
            throw RayTracer::SceneImportException(sceneFile, "Scene creation failed without specific error");
        auto camera = std::make_shared<RayTracer::Camera>(scene->getCamera());

        if (displayMode) {
            auto displayManager = std::make_shared<RayTracer::SFMLDisplayManager>();
            displayManager->initialize(image_width, image_height, "Raytracer", false);

            auto eventsManager = std::make_shared<RayTracer::SFMLEventsManager>(displayManager->getWindow());
            RayTracer::Renderer renderer(displayManager);

            RayTracer::InputManager inputManager(eventsManager, image_width, image_height);

            auto prims = scene->getPrimitives();

            while (displayManager->isWindowOpen()) {
                scene->setCamera(*camera);
                scene->updatePrimitiveCache();
                renderer.drawScene(*scene, *camera, inputManager.isMoving());
                inputManager.processInput(scene, camera);
                if (eventsManager->isKeyPressed("ESCAPE"))
                    displayManager->closeWindow();
            }

            if (!director.saveSceneToFile(*scene, sceneFile))
                std::cerr << "Failed to save the scene to " << sceneFile << std::endl;
        }
    } catch (const RayTracer::IException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "DEBUG: Exception caught in main: " << e.what() << std::endl;
        return e.getErrorCode();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 84;
    }
    return 0;
}
