// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plugin Loading Demo
*/

#include <string>
#include <iostream>
#include <memory>
#include <filesystem>
#include "Primitive/Plugin/PrimitivePluginManager.hpp"
#include "Primitive/PrimitiveFactory/PrimitiveFactory.hpp"
#include "Material/Material.hpp"

namespace RayTracer {

// Function to load primitives from plugins
void loadPrimitivePlugins() {
    PrimitivePluginManager* pluginManager = PrimitivePluginManager::getInstance();
    
    // Look for plugins in the plugins directory
    std::string pluginDir = "./plugins";
    
    // Check if directory exists
    if (!std::filesystem::exists(pluginDir)) {
        std::cout << "Creating plugins directory..." << std::endl;
        std::filesystem::create_directory(pluginDir);
    }
    
    // Load all .so files in the plugins directory
    for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".so") {
            std::string pluginPath = entry.path().string();
            std::cout << "Loading plugin: " << pluginPath << std::endl;
            
            if (pluginManager->loadPlugin(pluginPath)) {
                std::cout << "Successfully loaded plugin from " << pluginPath << std::endl;
            } else {
                std::cerr << "Failed to load plugin from " << pluginPath << std::endl;
            }
        }
    }
    
    // Print loaded plugins
    auto loadedPlugins = pluginManager->getLoadedPluginNames();
    std::cout << "Loaded plugins: ";
    for (const auto& name : loadedPlugins) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    
    // Register plugins with the primitive factory
    for (const auto& typeName : loadedPlugins) {
        PrimitiveFactory::registerPrimitive(typeName, 
            [pluginManager, typeName](const std::map<std::string, double>& params,
                const std::shared_ptr<Material>& material) {
                return pluginManager->createPrimitive(typeName, params, material);
            });
    }
}

}  // namespace RayTracer