// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneReloader implementation
*/
#include <iostream>
#include <memory>
#include <string>
#include "SceneReloader.hpp"

namespace RayTracer {

SceneReloader::SceneReloader(const std::string& filePath)
: filePath(filePath) {
    scene = std::shared_ptr<Scene>
        (director.createSceneFromFile(filePath).release());
    if (!scene)
        std::cerr << "Warning: Could not load scene from file: "
            << filePath << std::endl;
}

void SceneReloader::update() {
    std::cout << "Detected changes in scene file. Reloading..." << std::endl;
    try {
        auto newScene = director.createSceneFromFile(filePath);
        if (newScene) {
            scene = std::shared_ptr<Scene>(newScene.release());
            std::cout << "Scene reloaded successfully!" << std::endl;
        } else {
            std::cerr << "Error: Failed to reload scene." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error during scene reload: " << e.what() << std::endl;
    }
}

std::shared_ptr<Scene> SceneReloader::getScene() const {
    return scene;
}

}  // namespace RayTracer
