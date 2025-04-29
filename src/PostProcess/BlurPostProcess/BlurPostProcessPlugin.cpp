// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BlurPostProcessPlugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "PostProcess/Plugin/IPostProcessPlugin.hpp"
#include "BlurPostProcess.hpp"

namespace RayTracer {

class BlurPostProcessPlugin : public IPostProcessPlugin {
 public:
    ~BlurPostProcessPlugin() override = default;

    std::string getTypeName() const override {
        return BlurPostProcess::getTypeNameStatic();
    }

    std::shared_ptr<IPostProcess> createPostProcess(
        const std::map<std::string, double>& params) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }

        double radius = params.at("radius");
        return std::make_shared<BlurPostProcess>(radius);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"radius"};
    }

    std::shared_ptr<IPostProcess> createPostProcessFromSetting(
        const libconfig::Setting& setting) override {
        if (!setting.exists("radius")) {
            throw std::runtime_error("Missing required parameter: radius");
        }

        double radius = static_cast<double>(setting["radius"]);
        return std::make_shared<BlurPostProcess>(radius);
    }
};

extern "C" {
    IPostProcessPlugin* createPostProcessPlugin() {
        return new BlurPostProcessPlugin();
    }
}

}  // namespace RayTracer