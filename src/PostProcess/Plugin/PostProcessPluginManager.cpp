// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PostProcessPluginManager implementation
*/
#include <sys/stat.h>
#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "PostProcessPluginManager.hpp"

namespace RayTracer {

namespace {
    // Put helper functions in anonymous namespace to prevent linking conflicts
    bool fileExists(const std::string& path) {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }
}

PostProcessPluginManager* PostProcessPluginManager::instance = nullptr;

PostProcessPluginManager::~PostProcessPluginManager() {
    for (auto& [name, info] : loadedPlugins) {
        if (info.handle)
            dlclose(info.handle);
    }
}

PostProcessPluginManager* PostProcessPluginManager::getInstance() {
    if (!instance)
        instance = new PostProcessPluginManager();

    return instance;
}

bool PostProcessPluginManager::loadPlugin(const std::string& path) {
    if (!fileExists(path)) {
        std::cerr << "PostProcess plugin file does not exist: " << path << std::endl;
        return false;
    }

    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load PostProcess plugin: " << dlerror() << std::endl;
        return false;
    }

    dlerror();

    using CreatePluginFunc = IPostProcessPlugin*(*)();
    CreatePluginFunc createPlugin = reinterpret_cast<CreatePluginFunc>(
        dlsym(handle, "createPostProcessPlugin"));

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Failed to load symbol from PostProcess plugin: " << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }

    IPostProcessPlugin* rawPlugin = createPlugin();
    if (!rawPlugin) {
        std::cerr << "PostProcess plugin creation function returned nullptr" << std::endl;
        dlclose(handle);
        return false;
    }

    std::shared_ptr<IPostProcessPlugin> plugin(rawPlugin);

    std::string typeName = plugin->getTypeName();

    if (loadedPlugins.find(typeName) != loadedPlugins.end()) {
        std::cerr << "A PostProcess plugin with type name '" << typeName
                  << "' is already loaded" << std::endl;
        dlclose(handle);
        return false;
    }

    PluginInfo info;
    info.handle = handle;
    info.plugin = plugin;
    loadedPlugins[typeName] = info;

    std::cout << "Successfully loaded PostProcess plugin: " << typeName << std::endl;
    return true;
}

bool PostProcessPluginManager::unloadPlugin(const std::string& typeName) {
    auto it = loadedPlugins.find(typeName);
    if (it == loadedPlugins.end()) {
        std::cerr << "PostProcess plugin not found: " << typeName << std::endl;
        return false;
    }

    if (it->second.handle)
        dlclose(it->second.handle);

    loadedPlugins.erase(it);
    return true;
}

std::shared_ptr<IPostProcessPlugin> PostProcessPluginManager::getPlugin(
const std::string& typeName) {
    auto it = loadedPlugins.find(typeName);
    if (it != loadedPlugins.end()) {
        return it->second.plugin;
    }
    return nullptr;
}

std::vector<std::string> PostProcessPluginManager::getLoadedPluginNames() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : loadedPlugins) {
        names.push_back(name);
    }
    return names;
}

std::shared_ptr<IPostProcess> PostProcessPluginManager::createPostProcess(
const std::string& typeName,
const std::map<std::string, double>& params) {
    auto plugin = getPlugin(typeName);
    if (!plugin) {
        std::cerr << "PostProcess plugin not found: " << typeName << std::endl;
        return nullptr;
    }

    try {
        return plugin->createPostProcess(params);
    } catch (const std::exception& e) {
        std::cerr << "Error creating post-process effect: " << e.what() << std::endl;
        return nullptr;
    }
}

bool PostProcessPluginManager::loadAllPlugins(const std::string& directory) {
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        std::cerr << "Failed to open PostProcess plugin directory: "
            << directory << std::endl;
        return false;
    }

    bool success = true;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;

        if (filename == "." || filename == ".." ||
            filename.find(".so") == std::string::npos) {
            continue;
        }

        std::string fullPath = directory + "/" + filename;
        if (!loadPlugin(fullPath)) {
            std::cerr << "Failed to load PostProcess plugin: " << fullPath << std::endl;
            success = false;
        }
    }

    closedir(dir);
    return success;
}

}  // namespace RayTracer