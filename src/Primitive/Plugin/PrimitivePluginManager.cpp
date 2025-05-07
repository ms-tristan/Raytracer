// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Primitive Plugin Manager implementation
*/
#include <sys/stat.h>
#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "PrimitivePluginManager.hpp"
#include "Exception/PrimitiveNotFoundException.hpp"
#include "Exception/InvalidOperationException.hpp"

namespace RayTracer {

bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

PrimitivePluginManager* PrimitivePluginManager::instance = nullptr;

PrimitivePluginManager::~PrimitivePluginManager() {
    for (auto& [name, info] : loadedPlugins) {
        if (info.handle)
            dlclose(info.handle);
    }
}

PrimitivePluginManager* PrimitivePluginManager::getInstance() {
    if (!instance)
        instance = new PrimitivePluginManager();

    return instance;
}

bool PrimitivePluginManager::loadPlugin(const std::string& path) {
    if (!fileExists(path)) {
        std::cerr << "Plugin file does not exist: " << path << std::endl;
        return false;
    }

    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        return false;
    }

    dlerror();

    using CreatePluginFunc = IPrimitivePlugin*(*)();
    CreatePluginFunc createPlugin = reinterpret_cast<CreatePluginFunc>(
        dlsym(handle, "createPrimitivePlugin"));

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Failed to load symbol: " << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }

    IPrimitivePlugin* rawPlugin = createPlugin();
    if (!rawPlugin) {
        std::cerr << "Plugin creation function returned nullptr" << std::endl;
        dlclose(handle);
        return false;
    }

    std::shared_ptr<IPrimitivePlugin> plugin(rawPlugin);

    std::string typeName = plugin->getTypeName();

    if (loadedPlugins.find(typeName) != loadedPlugins.end()) {
        std::cerr << "A plugin with type name '" << typeName
                  << "' is already loaded" << std::endl;
        dlclose(handle);
        return false;
    }

    PluginInfo info;
    info.handle = handle;
    info.plugin = plugin;
    loadedPlugins[typeName] = info;

    return true;
}

bool PrimitivePluginManager::unloadPlugin(const std::string& typeName) {
    auto it = loadedPlugins.find(typeName);
    if (it == loadedPlugins.end()) {
        std::cerr << "Plugin not found: " << typeName << std::endl;
        return false;
    }

    if (it->second.handle)
        dlclose(it->second.handle);

    loadedPlugins.erase(it);
    return true;
}

std::shared_ptr<IPrimitivePlugin> PrimitivePluginManager::getPlugin(
const std::string& typeName) {
    auto it = loadedPlugins.find(typeName);
    if (it != loadedPlugins.end()) {
        return it->second.plugin;
    }
    throw PrimitiveNotFoundException(typeName);
}

std::vector<std::string> PrimitivePluginManager::getLoadedPluginNames() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : loadedPlugins) {
        names.push_back(name);
    }
    return names;
}

std::shared_ptr<IPrimitive> PrimitivePluginManager::createPrimitive(
const std::string& typeName,
const std::map<std::string, double>& params,
const std::shared_ptr<Material>& material) {
    auto plugin = getPlugin(typeName);
    if (!plugin) {
        std::cerr << "Plugin not found: " << typeName << std::endl;
        throw PrimitiveNotFoundException(typeName + " (plugin not loaded or does not exist)");
    }

    try {
        return plugin->createPrimitive(params, material);
    } catch (const IException& e) {
        std::cerr << "Error creating primitive " << typeName << ": " << e.what() << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Error creating primitive " << typeName << ": " << e.what() << std::endl;
        throw InvalidOperationException("Failed to create primitive of type '" +
                                       typeName + "': " + e.what());
    }
}

bool PrimitivePluginManager::loadAllPlugins(const std::string& directory) {
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        std::cerr << "Failed to open plugin directory: "
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
            std::cerr << "Failed to load plugin: " << fullPath << std::endl;
            success = false;
        }
    }

    closedir(dir);
    return success;
}

}  // namespace RayTracer
