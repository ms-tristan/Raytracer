// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Cylinder/Cylinder.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class CylinderPlugin : public IPrimitivePlugin {
 public:
    ~CylinderPlugin() override = default;

    std::string getTypeName() const override {
        return Cylinder::getTypeNameStatic();
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
        double height = params.at("height");

        return std::make_shared<Cylinder>(center, axis, radius,
            height, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "ax", "ay", "az", "radius", "height"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new CylinderPlugin();
    }
}

}  // namespace RayTracer
