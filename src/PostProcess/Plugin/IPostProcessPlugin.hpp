// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IPostProcessPlugin interface
*/

#ifndef SRC_POSTPROCESS_PLUGIN_IPOSTPROCESSPLUGIN_HPP_
#define SRC_POSTPROCESS_PLUGIN_IPOSTPROCESSPLUGIN_HPP_

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <libconfig.h++>
#include "PostProcess/IPostProcess.hpp"

namespace RayTracer {

class IPostProcessPlugin {
 public:
    virtual ~IPostProcessPlugin() = default;

    virtual std::string getTypeName() const = 0;

    virtual std::shared_ptr<IPostProcess> createPostProcess(
        const std::map<std::string, double>& params) = 0;

    virtual std::vector<std::string> getRequiredParameters() const = 0;

    virtual std::shared_ptr<IPostProcess> createPostProcessFromSetting(
        const libconfig::Setting& setting) = 0;
};

extern "C" {
    IPostProcessPlugin* createPostProcessPlugin();
}

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_PLUGIN_IPOSTPROCESSPLUGIN_HPP_
