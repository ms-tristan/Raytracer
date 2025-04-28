// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Box Plugin implementation
*/

#include <memory>
#include <string>
#include <iostream>
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
        return Box::getTypeNameStatic();
    }

    std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end())
                throw std::runtime_error("Missing required parameter: "
                    + param);
        }

        Math::Coords centerCoords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Coords dimensionsCoords {
            params.at("width") / 2.0,
            params.at("height") / 2.0,
            params.at("depth") / 2.0
        };

        Math::Point3D center(centerCoords);
        Math::Vector3D dimensions(dimensionsCoords);

        return std::make_shared<Box>(center, dimensions, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "width", "height", "depth"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new BoxPlugin();
    }
}

}  // namespace RayTracer
