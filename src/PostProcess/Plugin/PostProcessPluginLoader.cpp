// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PostProcess Plugin Loading implementation
*/
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <cstring>
#include "PostProcess/Plugin/PostProcessPluginManager.hpp"
#include "PostProcess/PostProcessFactory.hpp"
#include "PostProcess/Plugin/PostProcessPluginLoader.hpp"

namespace RayTracer {

namespace {
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
}  // namespace

void loadPostProcessPlugins() {
    PostProcessPluginManager* pluginManager =
        PostProcessPluginManager::getInstance();

    std::string pluginDir = "./plugins/postprocess";

    struct stat st;
    if (stat(pluginDir.c_str(), &st) != 0)
        mkdir(pluginDir.c_str(), 0755);

    DIR* dir = opendir(pluginDir.c_str());
    if (dir == nullptr) {
        std::cerr << "Failed to open PostProcess plugin directory: "
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

    PostProcessFactory postProcessFactory;
    for (const auto& typeName : loadedPlugins) {
        postProcessFactory.registerPostProcess(typeName,
            [pluginManager, typeName](const std::map<std::string, double>& params) {
                return pluginManager->createPostProcess(typeName, params);
            });
    }
}

}  // namespace RayTracer
