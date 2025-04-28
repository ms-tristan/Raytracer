// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrimitivePluginManager
*/

#ifndef SRC_PRIMITIVE_PLUGIN_PRIMITIVEPLUGINMANAGER_HPP_
    #define SRC_PRIMITIVE_PLUGIN_PRIMITIVEPLUGINMANAGER_HPP_
    #include <dlfcn.h>
    #include <string>
    #include <map>
    #include <memory>
    #include <vector>
    #include "IPrimitivePlugin.hpp"

namespace RayTracer {

class PrimitivePluginManager {
 private:
    struct PluginInfo {
        void* handle;
        std::shared_ptr<IPrimitivePlugin> plugin;
    };

    std::map<std::string, PluginInfo> loadedPlugins;
    static PrimitivePluginManager* instance;

    PrimitivePluginManager() = default;
    ~PrimitivePluginManager();

 public:
    PrimitivePluginManager(const PrimitivePluginManager&) = delete;
    PrimitivePluginManager& operator=(const PrimitivePluginManager&) = delete;

    static PrimitivePluginManager* getInstance();

    bool loadPlugin(const std::string& path);
    bool unloadPlugin(const std::string& typeName);
    std::shared_ptr<IPrimitivePlugin> getPlugin(const std::string& typeName);
    std::vector<std::string> getLoadedPluginNames() const;
    std::shared_ptr<IPrimitive> createPrimitive(
        const std::string& typeName,
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material);

    bool loadAllPlugins(const std::string& directory);
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_PLUGIN_PRIMITIVEPLUGINMANAGER_HPP_
