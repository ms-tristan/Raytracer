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
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/Torus/Torus.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class TorusPlugin : public IPrimitivePlugin {
 public:
    TorusPlugin() {
        std::cout << "[DEBUG] TorusPlugin constructed" << std::endl;
    }
    ~TorusPlugin() override = default;

    std::string getTypeName() const override {
        std::cout << "[DEBUG] TorusPlugin::getTypeName called" << std::endl;
        return Torus::getTypeNameStatic();
    }

    std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) override
    {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end())
                throw std::runtime_error("Missing required parameter: " + param);
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
        double majorRadius = params.at("majorRadius");
        double minorRadius = params.at("minorRadius");

        return std::make_shared<Torus>(center, axis, majorRadius, minorRadius, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "ax", "ay", "az", "majorRadius", "minorRadius"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        std::cout << "[DEBUG] createPrimitivePlugin called for Torus" << std::endl;
        return new TorusPlugin();
    }
}

} // namespace RayTracer