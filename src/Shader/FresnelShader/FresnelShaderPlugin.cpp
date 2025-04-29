// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** FresnelShaderPlugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Shader/Plugin/IShaderPlugin.hpp"
#include "FresnelShader.hpp"

namespace RayTracer {

class FresnelShaderPlugin : public IShaderPlugin {
 public:
    ~FresnelShaderPlugin() override = default;

    std::string getTypeName() const override {
        return "fresnel";
    }

    std::shared_ptr<IShader> createShader(
        const std::map<std::string, double>& params) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }

        double fresnelPower = params.at("fresnelPower");
        double rimColorR = params.at("rimColorR");
        double rimColorG = params.at("rimColorG");
        double rimColorB = params.at("rimColorB");

        Math::Vector3D rimColor(Math::Coords{rimColorR, rimColorG, rimColorB});
        return std::make_shared<FresnelShader>(fresnelPower, rimColor);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"fresnelPower", "rimColorR", "rimColorG", "rimColorB"};
    }
};

extern "C" {
    RayTracer::IShaderPlugin* createShaderPlugin() {
        return new FresnelShaderPlugin();
    }
}

}  // namespace RayTracer
