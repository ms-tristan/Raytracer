// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Torus Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <limits>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Torus/Torus.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Exception/ValueRangeException.hpp"
#include "Exception/ConfigParseException.hpp"

namespace RayTracer {

class TorusPlugin : public IPrimitivePlugin {
 public:
    ~TorusPlugin() override = default;

    std::string getTypeName() const override {
        return getTypeNameStatic();
    }

    static std::string getTypeNameStatic() {
        return "torus";
    }

    std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) override {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw ConfigParseException("Missing required parameter: " + param);
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
        double majorRadius = params.at("major_radius");
        double minorRadius = params.at("minor_radius");

        if (majorRadius <= 0)
            throw ValueRangeException("Torus major radius", majorRadius, 0.0, std::numeric_limits<double>::max());
        if (minorRadius <= 0)
            throw ValueRangeException("Torus minor radius", minorRadius, 0.0, std::numeric_limits<double>::max());
        if (minorRadius >= majorRadius)
            throw ValueRangeException("Torus minor radius", minorRadius, 0.0, majorRadius);

        return std::make_shared<Torus>(center, axis, majorRadius, minorRadius, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "ax", "ay", "az", "major_radius", "minor_radius"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new TorusPlugin();
    }
}

}  // namespace RayTracer
