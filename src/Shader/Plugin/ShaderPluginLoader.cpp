// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Shader Plugin Loading implementation
*/
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <cstring>
#include "Shader/Plugin/ShaderPluginManager.hpp"
#include "Shader/ShaderFactory.hpp"
#include "Shader/Plugin/ShaderPluginLoader.hpp"

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

void loadShaderPlugins() {
    ShaderPluginManager* pluginManager =
        ShaderPluginManager::getInstance();

    std::string pluginDir = "./plugins";

    struct stat st;
    if (stat(pluginDir.c_str(), &st) != 0)
        mkdir(pluginDir.c_str(), 0755);

    DIR* dir = opendir(pluginDir.c_str());
    if (dir == nullptr) {
        std::cerr << "Failed to open shader plugin directory: "
            << pluginDir << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string fileName = entry->d_name;
        std::string filePath = pluginDir + "/" + fileName;

        if (isRegularFile(filePath) && hasExtension(fileName, ".so")) {
            pluginManager->loadPlugin(filePath);
        }
    }
    closedir(dir);

    auto loadedPlugins = pluginManager->getLoadedPluginNames();
    std::cout << "Loaded " << loadedPlugins.size() << " shader plugins" << std::endl;

    ShaderFactory shaderFactory;
    for (const auto& typeName : loadedPlugins) {
        std::cout << "Registering shader plugin: " << typeName << std::endl;

        shaderFactory.registerShader(typeName,
            [pluginManager, typeName](const libconfig::Setting& setting) {
                std::map<std::string, double> params;

                auto plugin = pluginManager->getPlugin(typeName);
                if (plugin) {
                    auto requiredParams = plugin->getRequiredParameters();
                    for (const auto& param : requiredParams) {
                        if (setting.exists(param.c_str()))
                            params[param] = static_cast<double>(setting[param.c_str()]);
                    }
                }

                return pluginManager->createShader(typeName, params);
            });
    }
}

}  // namespace RayTracer
