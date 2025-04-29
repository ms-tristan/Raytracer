// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PostProcessFactory implementation
*/

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "PostProcess/PostProcessFactory.hpp"

namespace RayTracer {

std::map<std::string, PostProcessFactory::CreatorFunc> PostProcessFactory::creators;
bool PostProcessFactory::isInitialized = false;

std::shared_ptr<IPostProcess> PostProcessFactory::createPostProcess(
    const std::string& type,
    const std::map<std::string, double>& params) {
    auto it = creators.find(type);
    if (it == creators.end()) {
        std::cerr << "Unknown post-process type: " << type << std::endl;
        return nullptr;
    }

    return it->second(params);
}

void PostProcessFactory::registerPostProcess(
    const std::string& type, CreatorFunc creator) {
    creators[type] = creator;
}

std::vector<std::string> PostProcessFactory::getAvailablePostProcessTypes() {
    std::vector<std::string> types;
    for (const auto& [type, _] : creators) {
        types.push_back(type);
    }
    return types;
}

}  // namespace RayTracer
