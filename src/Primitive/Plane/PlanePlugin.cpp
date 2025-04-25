// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Plane Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Plane/Plane.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class PlanePlugin : public IPrimitivePlugin {
 public:
    ~PlanePlugin() override = default;

    std::string getTypeName() const override {
        return "plane";
    }

    std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: "
                    + param);
            }
        }

        Math::Coords posCoords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Coords normalCoords {
            params.at("nx"),
            params.at("ny"),
            params.at("nz")
        };
        Math::Point3D position(posCoords);
        Math::Vector3D normal(normalCoords);

        return std::make_shared<Plane>(position, normal, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "nx", "ny", "nz"};
    }
};

extern "C" {
    IPrimitivePlugin* createPlugin() {
        return new PlanePlugin();
    }
}

}  // namespace RayTracer
