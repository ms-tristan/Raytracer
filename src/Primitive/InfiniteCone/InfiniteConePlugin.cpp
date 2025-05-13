/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InfiniteCone Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/InfiniteCone/InfiniteCone.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class InfiniteConePlugin : public IPrimitivePlugin {
 public:
    ~InfiniteConePlugin() override = default;

    std::string getTypeName() const override {
        return InfiniteCone::getTypeNameStatic();
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

        Math::Coords apexCoords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Coords axisCoords {
            params.at("ax"),
            params.at("ay"),
            params.at("az")
        };

        Math::Point3D apex(apexCoords);
        Math::Vector3D axis(axisCoords);
        double angle = params.at("angle");

        return std::make_shared<InfiniteCone>(apex, axis, angle, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "ax", "ay", "az", "angle"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new InfiniteConePlugin();
    }
}

}  // namespace RayTracer
