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
#include <iostream>
#include <limits>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Sphere/Sphere.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Exception/ValueRangeException.hpp"
#include "Exception/ConfigParseException.hpp"
#include "Exception/InvalidOperationException.hpp"

namespace RayTracer {

class SpherePlugin : public IPrimitivePlugin {
 public:
    ~SpherePlugin() override = default;

    std::string getTypeName() const override {
        return getTypeNameStatic();
    }

    static std::string getTypeNameStatic() {
        return "spheres";
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

        Math::Coords coords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        Math::Point3D center(coords);
        double radius = params.at("radius");

        if (radius <= 0)
            throw ValueRangeException("Sphere radius", radius, 0.0, std::numeric_limits<double>::max());

        return std::make_shared<Sphere>(center, radius, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "radius"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new SpherePlugin();
    }
}

}  // namespace RayTracer
