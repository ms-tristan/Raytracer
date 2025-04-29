// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PostProcessPluginManager
*/

#ifndef SRC_POSTPROCESS_PLUGIN_POSTPROCESSPLUGINMANAGER_HPP_
#define SRC_POSTPROCESS_PLUGIN_POSTPROCESSPLUGINMANAGER_HPP_

#include <dlfcn.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "IPostProcessPlugin.hpp"

namespace RayTracer {

class PostProcessPluginManager {
 private:
    struct PluginInfo {
        void* handle;
        std::shared_ptr<IPostProcessPlugin> plugin;
    };

    std::map<std::string, PluginInfo> loadedPlugins;
    static PostProcessPluginManager* instance;

    PostProcessPluginManager() = default;
    ~PostProcessPluginManager();

 public:
    PostProcessPluginManager(const PostProcessPluginManager&) = delete;
    PostProcessPluginManager& operator=(const PostProcessPluginManager&) = delete;

    static PostProcessPluginManager* getInstance();

    bool loadPlugin(const std::string& path);
    bool unloadPlugin(const std::string& typeName);
    std::shared_ptr<IPostProcessPlugin> getPlugin(const std::string& typeName);
    std::vector<std::string> getLoadedPluginNames() const;
    std::shared_ptr<IPostProcess> createPostProcess(
        const std::string& typeName,
        const std::map<std::string, double>& params);

    bool loadAllPlugins(const std::string& directory);
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_PLUGIN_POSTPROCESSPLUGINMANAGER_HPP_