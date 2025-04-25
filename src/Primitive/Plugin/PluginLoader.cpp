// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plugin Loading Demo
*/
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <cstring>
#include "Primitive/Plugin/PrimitivePluginManager.hpp"
#include "Primitive/PrimitiveFactory/PrimitiveFactory.hpp"
#include "Material/Material.hpp"
#include "Primitive/Plugin/PluginLoader.hpp"

namespace RayTracer {

bool isRegularFile(const std::string& path) {
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0) {
        return false;
    }
    return S_ISREG(pathStat.st_mode);
}

bool hasExtension(const std::string& path, const std::string& ext) {
    const char* fileName = path.c_str();
    const char* extension = strrchr(fileName, '.');
    return extension != nullptr && strcmp(extension, ext.c_str()) == 0;
}

void loadPrimitivePlugins() {
    PrimitivePluginManager* pluginManager =
        PrimitivePluginManager::getInstance();

    std::string pluginDir = "./plugins";

    struct stat st;
    if (stat(pluginDir.c_str(), &st) != 0)
        mkdir(pluginDir.c_str(), 0755);

    DIR* dir = opendir(pluginDir.c_str());
    if (dir == nullptr) {
        std::cerr << "Failed to open plugin directory: "
            << pluginDir << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;
        std::string filePath = pluginDir + "/" + fileName;

        if (isRegularFile(filePath) && hasExtension(fileName, ".so")) {
            std::cout << "Loading plugin: " << filePath << std::endl;

            if (pluginManager->loadPlugin(filePath))
                std::cout << "Successfully loaded plugin from "
                    << filePath << std::endl;
            else
                std::cerr << "Failed to load plugin from "
                    << filePath << std::endl;
        }
    }
    closedir(dir);

    auto loadedPlugins = pluginManager->getLoadedPluginNames();
    std::cout << "Loaded plugins: ";
    for (const auto& name : loadedPlugins) {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    for (const auto& typeName : loadedPlugins) {
        PrimitiveFactory::registerPrimitive(typeName,
            [pluginManager, typeName](const std::map<std::string,
                double>& params,
                const std::shared_ptr<Material>& material) {
                return pluginManager->createPrimitive(typeName,
                    params, material);
            });
    }
}

}  // namespace RayTracer
