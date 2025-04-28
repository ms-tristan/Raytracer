// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PhongShaderPlugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Shader/Plugin/IShaderPlugin.hpp"
#include "PhongShader.hpp"

namespace RayTracer {

class PhongShaderPlugin : public IShaderPlugin {
 public:
    ~PhongShaderPlugin() override = default;

    std::string getTypeName() const override {
        return "phong";
    }

    std::shared_ptr<IShader> createShader(
        const std::map<std::string, double>& params) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }

        double specularStrength = params.at("specularStrength");
        double shininess = params.at("shininess");

        return std::make_shared<PhongShader>(specularStrength, shininess);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"specularStrength", "shininess"};
    }
};

extern "C" {
    RayTracer::IShaderPlugin* createShaderPlugin() {
        return new PhongShaderPlugin();
    }
}

}  // namespace RayTracer