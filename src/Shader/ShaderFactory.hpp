// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ShaderFactory - factory for creating shader objects
*/

#ifndef SRC_SHADER_SHADERFACTORY_HPP_
   #define SRC_SHADER_SHADERFACTORY_HPP_
   #include <functional>
   #include <memory>
   #include <string>
   #include <unordered_map>
   #include <libconfig.h++>
   #include "IShader.hpp"

namespace RayTracer {

class ShaderFactory {
 public:
    using ShaderCreator = std::function<std::shared_ptr<IShader>(const libconfig::Setting&)>;

    ShaderFactory();
    ~ShaderFactory() = default;

    std::shared_ptr<IShader> createShader(const std::string& type, const libconfig::Setting& setting);
    std::shared_ptr<IShader> createShaderFromSetting(const libconfig::Setting& setting);

    void registerShader(const std::string& type, ShaderCreator creator);

 private:
    std::unordered_map<std::string, ShaderCreator> shaderCreators;

    void registerBuiltInShaders();
};

}  // namespace RayTracer

#endif  // SRC_SHADER_SHADERFACTORY_HPP_
