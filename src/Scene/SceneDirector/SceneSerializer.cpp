/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneSerializer
*/
#include <string>
#include <iostream>
#include <memory>
#include <libconfig.h++>
#include "SceneSerializer.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/PointLight/PointLight.hpp"

namespace RayTracer {

bool SceneSerializer::saveToFile(const Scene& scene, const std::string& filename) {
    try {
        libconfig::Config cfg;
        libconfig::Setting& root = cfg.getRoot();

        std::shared_ptr<libconfig::Setting> rootPtr(&root, [](libconfig::Setting*){});
        scene.getLibConfigParams(rootPtr);

        cfg.writeFile(filename.c_str());

        return true;
    } catch (const libconfig::FileIOException& ex) {
        std::cerr << "Error writing scene file: " << filename << std::endl;
        return false;
    } catch (const std::exception& ex) {
        std::cerr << "Error serializing scene: " << ex.what() << std::endl;
        return false;
    }
}

}  // namespace RayTracer
