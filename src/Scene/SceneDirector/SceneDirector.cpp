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
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Material/Material.hpp"

namespace RayTracer {

std::unique_ptr<Scene> SceneDirector::createDefaultScene() {
    return createBasicSphereScene();
}

std::unique_ptr<Scene> SceneDirector::createBasicSphereScene() {
    const Math::Coords red_color{0.9, 0.2, 0.2};
    auto redMaterial = std::make_shared<Material>();
    redMaterial->color = Math::Vector3D(red_color);

    const Math::Coords white_color{0.8, 0.8, 0.8};
    auto whiteMaterial = std::make_shared<Material>();
    whiteMaterial->color = Math::Vector3D(white_color);

    const Math::Coords full_white{1.0, 1.0, 1.0};
    const Math::Coords light_coords{0, 5, 5};

    auto directionalLight = std::make_shared<DirectionalLight>(
        Math::Vector3D(light_coords),
        Math::Vector3D(full_white));

    return builder.reset()
        .setCamera(Math::Point3D(Math::Coords{0, 1, 5}),
            Math::Point3D(Math::Coords{0, 0, 0}))
        .setAmbientLight(Math::Vector3D(Math::Coords{0.2, 0.2, 0.2}))
        .addLight(directionalLight)
        .addSphere(Math::Point3D(Math::Coords{0, 0, 0}), 1.0, redMaterial)
        .addPlane(Math::Point3D(Math::Coords{0, -1, 0}),
            Math::Vector3D(Math::Coords{0, 1, 0}), whiteMaterial)
        .build();
}

std::unique_ptr<Scene> SceneDirector::createSceneFromFile(
const std::string& filename) {
    try {
        // Use our new config parser to load the scene from the file
        SceneConfigParser parser;
        return parser.parseFile(filename);
    } catch (const std::exception& ex) {
        std::cerr << "Error creating scene from file: " << ex.what() << std::endl;
        return createDefaultScene();
    }
}

}  // namespace RayTracer
