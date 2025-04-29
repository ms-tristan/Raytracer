// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ChromaticAberrationPostProcessPlugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "PostProcess/Plugin/IPostProcessPlugin.hpp"
#include "ChromaticAberrationPostProcess.hpp"

namespace RayTracer {

class ChromaticAberrationPostProcessPlugin : public IPostProcessPlugin {
 public:
    ~ChromaticAberrationPostProcessPlugin() override = default;

    std::string getTypeName() const override {
        return ChromaticAberrationPostProcess::getTypeNameStatic();
    }

    std::shared_ptr<IPostProcess> createPostProcess(
        const std::map<std::string, double>& params) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }

        double strength = params.at("strength");
        return std::make_shared<ChromaticAberrationPostProcess>(strength);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"strength"};
    }

    std::shared_ptr<IPostProcess> createPostProcessFromSetting(
        const libconfig::Setting& setting) override {
        if (!setting.exists("strength")) {
            throw std::runtime_error("Missing required parameter: strength");
        }
        double strength = static_cast<double>(setting["strength"]);
        return std::make_shared<ChromaticAberrationPostProcess>(strength);
    }
};

extern "C" {
    IPostProcessPlugin* createPostProcessPlugin() {
        return new ChromaticAberrationPostProcessPlugin();
    }
}

}  // namespace RayTracer
