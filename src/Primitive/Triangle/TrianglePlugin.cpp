// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Triangle Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Triangle/Triangle.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class TrianglePlugin : public IPrimitivePlugin {
 public:
    ~TrianglePlugin() override = default;

    std::string getTypeName() const override {
        return Triangle::getTypeNameStatic();
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

        Math::Coords v1Coords {
            params.at("v1x"),
            params.at("v1y"),
            params.at("v1z")
        };
        Math::Coords v2Coords {
            params.at("v2x"),
            params.at("v2y"),
            params.at("v2z")
        };
        Math::Coords v3Coords {
            params.at("v3x"),
            params.at("v3y"),
            params.at("v3z")
        };
        Math::Point3D v1(v1Coords);
        Math::Point3D v2(v2Coords);
        Math::Point3D v3(v3Coords);

        return std::make_shared<Triangle>(v1, v2, v3, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"v1x", "v1y", "v1z", "v2x", "v2y", "v2z", "v3x", "v3y", "v3z"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new TrianglePlugin();
    }
}

}  // namespace RayTracer
