// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrimitiveFactory
*/
#include <stdexcept>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "PrimitiveFactory.hpp"

namespace RayTracer {

std::map<std::string, PrimitiveFactory::CreatorFunc> PrimitiveFactory::creators;
bool PrimitiveFactory::isInitialized = false;

std::shared_ptr<IPrimitive> PrimitiveFactory::createPrimitive(
const std::string& type,
const std::map<std::string, double>& params,
const std::shared_ptr<Material>& material) {
    auto it = creators.find(type);
    if (it == creators.end()) {
        throw std::runtime_error("Unknown primitive type: " + type);
    }

    return it->second(params, material);
}

void PrimitiveFactory::registerPrimitive(const std::string& type,
CreatorFunc creator) {
    creators[type] = creator;
}

std::vector<std::string> PrimitiveFactory::getAvailablePrimitiveTypes() {
    std::vector<std::string> types;
    for (const auto& [type, _] : creators) {
        types.push_back(type);
    }
    return types;
}

// Remove the built-in primitives registration as these will be handled by plugins
void PrimitiveFactory::registerBuiltInPrimitives() {
    // This function is now empty as we'll rely on plugins
    // The primitive implementations will be registered by the plugin system
}

}  // namespace RayTracer
