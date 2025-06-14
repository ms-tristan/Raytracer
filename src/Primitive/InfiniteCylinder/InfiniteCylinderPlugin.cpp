/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** InfiniteCylinder Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/InfiniteCylinder/InfiniteCylinder.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class InfiniteCylinderPlugin : public IPrimitivePlugin {
 public:
    ~InfiniteCylinderPlugin() override = default;

    std::string getTypeName() const override {
        return InfiniteCylinder::getTypeNameStatic();
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

        Math::Coords centerCoords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Coords axisCoords {
            params.at("ax"),
            params.at("ay"),
            params.at("az")
        };

        Math::Point3D center(centerCoords);
        Math::Vector3D axis(axisCoords);
        double radius = params.at("radius");

        return std::make_shared<InfiniteCylinder>(center, axis, radius, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "ax", "ay", "az", "radius"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new InfiniteCylinderPlugin();
    }
}

}  // namespace RayTracer
