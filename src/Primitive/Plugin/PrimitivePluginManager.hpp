#ifndef SRC_PRIMITIVE_PLUGIN_PRIMITIVEPLUGINMANAGER_HPP_
#define SRC_PRIMITIVE_PLUGIN_PRIMITIVEPLUGINMANAGER_HPP_

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <dlfcn.h>
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

    // Load a plugin from a shared library file
    bool loadPlugin(const std::string& path);
    
    // Unload a specific plugin by name
    bool unloadPlugin(const std::string& typeName);
    
    // Get a plugin by its type name
    std::shared_ptr<IPrimitivePlugin> getPlugin(const std::string& typeName);
    
    // Get all loaded plugin names
    std::vector<std::string> getLoadedPluginNames() const;
    
    // Create a primitive using a plugin
    std::shared_ptr<IPrimitive> createPrimitive(
        const std::string& typeName,
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material);
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_PLUGIN_PRIMITIVEPLUGINMANAGER_HPP_