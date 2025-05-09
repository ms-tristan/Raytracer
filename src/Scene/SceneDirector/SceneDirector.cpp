// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneDirector
*/
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <memory>
#include "SceneDirector.hpp"
#include "ConfigParser.hpp"
#include "SceneSerializer.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Material/Material.hpp"
#include "Exception/SceneImportException.hpp"

namespace RayTracer {

std::shared_ptr<Scene> SceneDirector::createDefaultScene() {
    return createBasicSphereScene();
}

std::shared_ptr<Scene> SceneDirector::createBasicSphereScene() {
    const Math::Coords red_color{0.9, 0.2, 0.2};
    auto redMaterial = std::make_shared<Material>();
    redMaterial->color = Math::Vector3D(red_color);

    const Math::Coords white_color{0.8, 0.8, 0.8};
    auto whiteMaterial = std::make_shared<Material>();
    whiteMaterial->color = Math::Vector3D(white_color);

    const Math::Coords full_white{1.0, 1.0, 1.0};
    const Math::Coords light_blue{0.1, 0.0, 0.0};

    const Math::Coords light1_coords{30, -10, 0};
    const Math::Coords light2_coords{-5, -10, 0};

    auto directionalLight1 = std::make_shared<DirectionalLight>(
        Math::Vector3D(light2_coords),
        Math::Vector3D(full_white));

    auto directionalLight2 = std::make_shared<DirectionalLight>(
        Math::Vector3D(light1_coords),
        Math::Vector3D(full_white));

    return builder.reset()
        .setCamera(Math::Point3D(Math::Coords{0, 1, 5}),
            Math::Point3D(Math::Coords{0, 0, 0}))
        .setAmbientLight(Math::Vector3D(Math::Coords{0.2, 0.2, 0.2}))
        .addLight(directionalLight1)
        .addLight(directionalLight2)
        .addSphere(Math::Point3D(Math::Coords{0, 0, 0}), 1.0, redMaterial)
        .addPlane(Math::Point3D(Math::Coords{0, -1, 0}),
            Math::Vector3D(Math::Coords{0, 1, 0}), whiteMaterial)
        .build();
}

std::shared_ptr<Scene> SceneDirector::createSceneFromFile(
const std::string& filename) {
    try {
        currentSceneFile = filename;

        SceneConfigParser parser;
        auto scene = parser.parseFile(filename);
        if (!scene) {
            throw SceneImportException(filename, "Failed to parse scene file - no scene was created");
        }
        return scene;
    } catch (const IException& ex) {
        throw;
    } catch (const std::exception& ex) {
        throw SceneImportException(filename, "Failed to load scene: " + std::string(ex.what()));
    }
}

bool SceneDirector::saveSceneToFile(const Scene& scene, const std::string& filename) {
    SceneSerializer serializer;
    bool success = serializer.saveToFile(scene, filename);
    if (success) {
        std::cout << "Scene saved to file: " << filename << std::endl;
    }
    return success;
}

bool SceneDirector::saveScene(const Scene& scene) {
    if (currentSceneFile.empty()) {
        std::cerr << "No scene file to save to. The scene was not loaded from a file." << std::endl;
        return false;
    }

    return saveSceneToFile(scene, currentSceneFile);
}

}  // namespace RayTracer
