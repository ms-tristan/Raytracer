// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Sphere Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Sphere/Sphere.hpp"
#include "Math/Point3D/Point3D.hpp"

namespace RayTracer {

class SpherePlugin : public IPrimitivePlugin {
 public:
    ~SpherePlugin() override = default;

    std::string getTypeName() const override {
        return "sphere";
    }

    std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) override {
        
        // Check all required parameters are present
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }

        // Extract parameters
        Math::Coords coords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Point3D center(coords);
        double radius = params.at("radius");

        // Create and return the sphere
        return std::make_shared<Sphere>(center, radius, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "radius"};
    }
};

// Create the plugin entry point function
extern "C" {
    IPrimitivePlugin* createPlugin() {
        return new SpherePlugin();
    }
}

}  // namespace RayTracer