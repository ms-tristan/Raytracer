// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** GrayscalePostProcessPlugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "PostProcess/Plugin/IPostProcessPlugin.hpp"
#include "GrayscalePostProcess.hpp"

namespace RayTracer {

class GrayscalePostProcessPlugin : public IPostProcessPlugin {
 public:
    ~GrayscalePostProcessPlugin() override = default;

    std::string getTypeName() const override {
        return GrayscalePostProcess::getTypeNameStatic();
    }

    std::shared_ptr<IPostProcess> createPostProcess(
        const std::map<std::string, double>& params) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }

        double intensity = params.at("intensity");
        return std::make_shared<GrayscalePostProcess>(intensity);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"intensity"};
    }

    std::shared_ptr<IPostProcess> createPostProcessFromSetting(
        const libconfig::Setting& setting) override {
        if (!setting.exists("intensity")) {
            throw std::runtime_error("Missing required parameter: intensity");
        }

        double intensity = static_cast<double>(setting["intensity"]);
        return std::make_shared<GrayscalePostProcess>(intensity);
    }
};

extern "C" {
    IPostProcessPlugin* createPostProcessPlugin() {
        return new GrayscalePostProcessPlugin();
    }
}

}  // namespace RayTracer
