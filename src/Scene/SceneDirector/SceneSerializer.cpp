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
#include "Scene/SceneBuilder/SceneBuilder.hpp"
#include "Scene/SceneDirector/ObjModelInfo.hpp"

namespace RayTracer {

bool SceneSerializer::saveToFile(const Scene& scene, const std::string& filename) {
    try {
        libconfig::Config cfg;
        libconfig::Setting& root = cfg.getRoot();

        std::shared_ptr<libconfig::Setting> rootPtr(&root, [](libconfig::Setting*){});
        scene.getLibConfigParams(rootPtr);

        const auto& objInfos = scene.getObjModelInfos();
        if (!objInfos.empty()) {
            libconfig::Setting& primGroup = root["primitives"];
            libconfig::Setting& objList = primGroup.exists("obj_model") ? primGroup["obj_model"] : primGroup.add("obj_model", libconfig::Setting::TypeList);
            for (const auto& info : objInfos) {
                libconfig::Setting& obj = objList.add(libconfig::Setting::TypeGroup);
                obj.add("path", libconfig::Setting::TypeString) = info.path;
                libconfig::Setting& pos = obj.add("position", libconfig::Setting::TypeGroup);
                pos.add("x", libconfig::Setting::TypeFloat) = info.position.X;
                pos.add("y", libconfig::Setting::TypeFloat) = info.position.Y;
                pos.add("z", libconfig::Setting::TypeFloat) = info.position.Z;
                if (info.material) {
                    libconfig::Setting& mat = obj.add("material", libconfig::Setting::TypeGroup);
                    libconfig::Setting& color = mat.add("color", libconfig::Setting::TypeGroup);
                    color.add("r", libconfig::Setting::TypeFloat) = info.material->color.X;
                    color.add("g", libconfig::Setting::TypeFloat) = info.material->color.Y;
                    color.add("b", libconfig::Setting::TypeFloat) = info.material->color.Z;
                    mat.add("reflectivity", libconfig::Setting::TypeFloat) = info.material->reflectivity;
                }
                if (info.rotation.X != 0.0 || info.rotation.Y != 0.0 || info.rotation.Z != 0.0) {
                    libconfig::Setting& rot = obj.add("rotation", libconfig::Setting::TypeGroup);
                    rot.add("x", libconfig::Setting::TypeFloat) = info.rotation.X;
                    rot.add("y", libconfig::Setting::TypeFloat) = info.rotation.Y;
                    rot.add("z", libconfig::Setting::TypeFloat) = info.rotation.Z;
                }
                if (info.scale != 1.0)
                    obj.add("scale", libconfig::Setting::TypeFloat) = info.scale;
            }
        }

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
