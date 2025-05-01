// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Mobius Strip Plugin implementation
*/

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/MobiusStrip/MobiusStrip.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class MobiusStripPlugin : public IPrimitivePlugin {
 public:
    ~MobiusStripPlugin() override = default;

    std::string getTypeName() const override {
        return MobiusStrip::getTypeNameStatic();
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
        Math::Point3D center(centerCoords);
        double majorRadius = params.at("major_radius");
        double minorRadius = params.at("minor_radius");
        double thickness = params.at("thickness");

        return std::make_shared<MobiusStrip>(center, majorRadius, minorRadius, thickness, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "major_radius", "minor_radius", "thickness"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new MobiusStripPlugin();
    }
}

}  // namespace RayTracer