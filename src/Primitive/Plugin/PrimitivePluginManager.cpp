// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Primitive Plugin Manager implementation
*/

#include <iostream>
#include <filesystem>
#include "PrimitivePluginManager.hpp"

namespace RayTracer {

PrimitivePluginManager* PrimitivePluginManager::instance = nullptr;

PrimitivePluginManager::~PrimitivePluginManager() {
    // Unload all plugins
    for (auto& [name, info] : loadedPlugins) {
        if (info.handle) {
            dlclose(info.handle);
        }
    }
}

PrimitivePluginManager* PrimitivePluginManager::getInstance() {
    if (!instance) {
        instance = new PrimitivePluginManager();
    }
    return instance;
}

bool PrimitivePluginManager::loadPlugin(const std::string& path) {
    // Check if file exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "Plugin file does not exist: " << path << std::endl;
        return false;
    }

    // Open the dynamic library
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        return false;
    }

    // Reset errors
    dlerror();

    // Load the creation function
    using CreatePluginFunc = IPrimitivePlugin*(*)();
    CreatePluginFunc createPlugin = reinterpret_cast<CreatePluginFunc>(
        dlsym(handle, "createPlugin"));
    
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Failed to load symbol: " << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }

    // Create the plugin
    IPrimitivePlugin* rawPlugin = createPlugin();
    if (!rawPlugin) {
        std::cerr << "Plugin creation function returned nullptr" << std::endl;
        dlclose(handle);
        return false;
    }

    // Wrap in shared_ptr with custom deleter
    std::shared_ptr<IPrimitivePlugin> plugin(rawPlugin);
    
    // Get the plugin's type name
    std::string typeName = plugin->getTypeName();
    
    // Check if a plugin with this name is already loaded
    if (loadedPlugins.find(typeName) != loadedPlugins.end()) {
        std::cerr << "A plugin with type name '" << typeName 
                  << "' is already loaded" << std::endl;
        dlclose(handle);
        return false;
    }
    
    // Store the plugin
    PluginInfo info;
    info.handle = handle;
    info.plugin = plugin;
    loadedPlugins[typeName] = info;
    
    std::cout << "Successfully loaded plugin: " << typeName << std::endl;
    return true;
}

bool PrimitivePluginManager::unloadPlugin(const std::string& typeName) {
    auto it = loadedPlugins.find(typeName);
    if (it == loadedPlugins.end()) {
        std::cerr << "Plugin not found: " << typeName << std::endl;
        return false;
    }
    
    if (it->second.handle) {
        dlclose(it->second.handle);
    }
    
    loadedPlugins.erase(it);
    return true;
}

std::shared_ptr<IPrimitivePlugin> PrimitivePluginManager::getPlugin(
    const std::string& typeName) {
    auto it = loadedPlugins.find(typeName);
    if (it != loadedPlugins.end()) {
        return it->second.plugin;
    }
    return nullptr;
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
        return nullptr;
    }
    
    try {
        return plugin->createPrimitive(params, material);
    } catch (const std::exception& e) {
        std::cerr << "Error creating primitive: " << e.what() << std::endl;
        return nullptr;
    }
}

}  // namespace RayTracer