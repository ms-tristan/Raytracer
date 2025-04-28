// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ToonShaderPlugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Shader/Plugin/IShaderPlugin.hpp"
#include "ToonShader.hpp"

namespace RayTracer {

class ToonShaderPlugin : public IShaderPlugin {
 public:
    ~ToonShaderPlugin() override = default;

    std::string getTypeName() const override {
        return "toon";
    }

    std::shared_ptr<IShader> createShader(
        const std::map<std::string, double>& params) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }

        int levels = static_cast<int>(params.at("levels"));
        double edgeThreshold = params.at("edgeThreshold");

        return std::make_shared<ToonShader>(levels, edgeThreshold);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"levels", "edgeThreshold"};
    }
};

extern "C" {
    RayTracer::IShaderPlugin* createShaderPlugin() {
        return new ToonShaderPlugin();
    }
}

}  // namespace RayTracer