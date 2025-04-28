// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ShaderPluginManager
*/

#ifndef SRC_SHADER_PLUGIN_SHADERPLUGINMANAGER_HPP_
#define SRC_SHADER_PLUGIN_SHADERPLUGINMANAGER_HPP_

#include <dlfcn.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "IShaderPlugin.hpp"

namespace RayTracer {

class ShaderPluginManager {
 private:
    struct PluginInfo {
        void* handle;
        std::shared_ptr<IShaderPlugin> plugin;
    };

    std::map<std::string, PluginInfo> loadedPlugins;
    static ShaderPluginManager* instance;

    ShaderPluginManager() = default;
    ~ShaderPluginManager();

 public:
    ShaderPluginManager(const ShaderPluginManager&) = delete;
    ShaderPluginManager& operator=(const ShaderPluginManager&) = delete;

    static ShaderPluginManager* getInstance();

    bool loadPlugin(const std::string& path);
    bool unloadPlugin(const std::string& typeName);
    std::shared_ptr<IShaderPlugin> getPlugin(const std::string& typeName);
    std::vector<std::string> getLoadedPluginNames() const;
    std::shared_ptr<IShader> createShader(
        const std::string& typeName,
        const std::map<std::string, double>& params);

    bool loadAllPlugins(const std::string& directory);
};

}  // namespace RayTracer

#endif  // SRC_SHADER_PLUGIN_SHADERPLUGINMANAGER_HPP_