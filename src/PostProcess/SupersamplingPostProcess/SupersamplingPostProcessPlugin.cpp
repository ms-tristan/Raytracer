// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SupersamplingPostProcessPlugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "PostProcess/Plugin/IPostProcessPlugin.hpp"
#include "SupersamplingPostProcess.hpp"
#include "PostProcess/SupersamplingPostProcess/SupersamplingPostProcessPlugin.hpp"

namespace RayTracer {

std::string SupersamplingPostProcessPlugin::getTypeName() const {
    return SupersamplingPostProcess::getTypeNameStatic();
}

std::shared_ptr<IPostProcess> SupersamplingPostProcessPlugin::createPostProcess(
    const std::map<std::string, double>& params) {
    auto requiredParams = getRequiredParameters();
    for (const auto& param : requiredParams) {
        if (params.find(param) == params.end()) {
            throw std::runtime_error("Missing required parameter: " + param);
        }
    }

    int samplesPerPixel = static_cast<int>(params.at("samplesPerPixel"));
    return std::make_shared<SupersamplingPostProcess>(samplesPerPixel);
}

std::vector<std::string> SupersamplingPostProcessPlugin::getRequiredParameters() const {
    return {"samplesPerPixel"};
}

std::shared_ptr<IPostProcess> SupersamplingPostProcessPlugin::createPostProcessFromSetting(
    const libconfig::Setting& setting) {
    if (!setting.exists("samplesPerPixel")) {
        throw std::runtime_error("Missing required parameter: samplesPerPixel");
    }

    int samplesPerPixel = static_cast<int>(setting["samplesPerPixel"]);
    return std::make_shared<SupersamplingPostProcess>(samplesPerPixel);
}

} // namespace RayTracer

extern "C" {
    RayTracer::IPostProcessPlugin* createPostProcessPlugin() {
        return new RayTracer::SupersamplingPostProcessPlugin();
    }
}
