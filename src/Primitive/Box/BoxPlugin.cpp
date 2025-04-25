// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Box Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Box/Box.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class BoxPlugin : public IPrimitivePlugin {
 public:
    ~BoxPlugin() override = default;

    std::string getTypeName() const override {
        return "box";
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
        Math::Coords centerCoords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Coords dimensionsCoords {
            params.at("width") / 2.0,  // Convert to half-width
            params.at("height") / 2.0, // Convert to half-height
            params.at("depth") / 2.0   // Convert to half-depth
        };
        
        Math::Point3D center(centerCoords);
        Math::Vector3D dimensions(dimensionsCoords);

        // Create and return the box
        return std::make_shared<Box>(center, dimensions, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "width", "height", "depth"};
    }
};

// Create the plugin entry point function
extern "C" {
    IPrimitivePlugin* createPlugin() {
        return new BoxPlugin();
    }
}

}  // namespace RayTracer