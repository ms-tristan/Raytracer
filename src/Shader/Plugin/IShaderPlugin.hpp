// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IShaderPlugin
*/

#ifndef SRC_SHADER_PLUGIN_ISHADERPLUGIN_HPP_
#define SRC_SHADER_PLUGIN_ISHADERPLUGIN_HPP_

#include <memory>
#include <vector>
#include <string>
#include <map>
#include "Shader/IShader.hpp"

namespace RayTracer {

class IShaderPlugin {
 public:
    virtual ~IShaderPlugin() = default;

    virtual std::string getTypeName() const = 0;

    virtual std::shared_ptr<IShader> createShader(
        const std::map<std::string, double>& params) = 0;

    virtual std::vector<std::string> getRequiredParameters() const = 0;
};

extern "C" {
    IShaderPlugin* createShaderPlugin();
}

}  // namespace RayTracer

#endif  // SRC_SHADER_PLUGIN_ISHADERPLUGIN_HPP_
