// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory implementation
*/
#include <stdexcept>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "LightFactory.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/PointLight/PointLight.hpp"

namespace RayTracer {

std::map<std::string, LightFactory::CreatorFunc> LightFactory::creators;
bool LightFactory::isInitialized = false;

std::shared_ptr<ILight> LightFactory::createLight(
const std::string& type,
const std::map<std::string, double>& params) {
    if (!isInitialized) {
        registerBuiltInLights();
        isInitialized = true;
    }

    auto it = creators.find(type);
    if (it == creators.end()) {
        throw std::runtime_error("Unknown light type: " + type);
    }

    return it->second(params);
}

void LightFactory::registerLight(const std::string& type,
CreatorFunc creator) {
    creators[type] = creator;
}

std::vector<std::string> LightFactory::getAvailableLightTypes() {
    if (!isInitialized) {
        registerBuiltInLights();
        isInitialized = true;
    }

    std::vector<std::string> types;
    for (const auto& [type, _] : creators) {
        types.push_back(type);
    }
    return types;
}

AmbientLight LightFactory::createAmbientLight(const Math::Vector3D& color) {
    return AmbientLight(color);
}

void LightFactory::registerBuiltInLights() {
    registerLight("directional", [](const std::map<std::string,
        double>& params) {
        const Math::Coords dir_coords {
            params.at("dx"),
            params.at("dy"),
            params.at("dz")
        };
        const Math::Coords color_coords {
            params.at("r") / 255.0,
            params.at("g") / 255.0,
            params.at("b") / 255.0
        };
        Math::Vector3D direction(dir_coords);
        Math::Vector3D color(color_coords);
        return std::make_shared<DirectionalLight>(direction, color);
    });

    registerLight("point", [](const std::map<std::string, double>& params) {
        const Math::Coords pos_coords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        const Math::Coords color_coords {
            params.at("r") / 255.0,
            params.at("g") / 255.0,
            params.at("b") / 255.0
        };
        Math::Point3D position(pos_coords);
        Math::Vector3D color(color_coords);
        double constant = params.count("constant") ?
            params.at("constant") : 1.0;
        double linear = params.count("linear") ? params.at("linear") : 0.09;
        double quadratic = params.count("quadratic") ?
            params.at("quadratic") : 0.032;
        return std::make_shared<PointLight>(position, color,
            constant, linear, quadratic);
    });
}

}  // namespace RayTracer
