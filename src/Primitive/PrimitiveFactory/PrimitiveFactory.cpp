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
#include "Primitive/Sphere/Sphere.hpp"
#include "Primitive/Plane/Plane.hpp"
#include "Primitive/Cylinder/Cylinder.hpp"
#include "Primitive/Cone/Cone.hpp"

namespace RayTracer {

std::map<std::string, PrimitiveFactory::CreatorFunc> PrimitiveFactory::creators;
bool PrimitiveFactory::isInitialized = false;

std::shared_ptr<IPrimitive> PrimitiveFactory::createPrimitive(
const std::string& type,
const std::map<std::string, double>& params,
const std::shared_ptr<Material>& material) {
    if (!isInitialized) {
        registerBuiltInPrimitives();
        isInitialized = true;
    }

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
    if (!isInitialized) {
        registerBuiltInPrimitives();
        isInitialized = true;
    }

    std::vector<std::string> types;
    for (const auto& [type, _] : creators) {
        types.push_back(type);
    }
    return types;
}

void PrimitiveFactory::registerBuiltInPrimitives() {
    registerPrimitive("sphere", [](const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) {
        const Math::Coords coords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Point3D center(coords);
        double radius = params.at("radius");
        return std::make_shared<Sphere>(center, radius, material);
    });

    registerPrimitive("plane", [](const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) {
        const Math::Coords coords_p {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        const Math::Coords coords_n {
            params.at("nx"),
            params.at("ny"),
            params.at("nz")
        };
        Math::Point3D position(coords_p);
        Math::Vector3D normal(coords_n);
        return std::make_shared<Plane>(position, normal, material);
    });

    registerPrimitive("cylinder", [](const std::map<std::string, double>&
        params, const std::shared_ptr<Material>& material) {
        const Math::Coords coords_p {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        const Math::Coords coords_a {
            params.at("ax"),
            params.at("ay"),
            params.at("az")
        };
        Math::Point3D center(coords_p);
        Math::Vector3D axis(coords_a);
        double radius = params.at("radius");
        double height = params.at("height");
        return std::make_shared<Cylinder>(center, axis, radius,
            height, material);
    });

    registerPrimitive("cone", [](const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) {
        const Math::Coords coords_p {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        const Math::Coords coords_a {
            params.at("ax"),
            params.at("ay"),
            params.at("az")
        };
        Math::Point3D apex(coords_p);
        Math::Vector3D axis(coords_a);
        double radius = params.at("radius");
        double height = params.at("height");
        return std::make_shared<Cone>(apex, axis, radius,
            height, material);
    });
}

}  // namespace RayTracer
