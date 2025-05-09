// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Klein Bottle Plugin implementation
*/
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/KleinBottle/KleinBottle.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class KleinBottlePlugin : public IPrimitivePlugin {
 public:
    ~KleinBottlePlugin() override = default;

    std::string getTypeName() const override {
        return KleinBottle::getTypeNameStatic();
    }

    std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) override {
        validateParameters(params);
        Math::Point3D center = createCenter(params);
        double scale = params.at("scale");
        double thickness = params.at("thickness");

        bool isFigure8 = false;
        if (params.find("figure8") != params.end())
            isFigure8 = params.at("figure8") > 0.5;

        return std::make_shared<KleinBottle>(center, scale, thickness, isFigure8, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "scale", "thickness"};
    }

 private:
    void validateParameters(const std::map<std::string, double>& params) const {
        auto requiredParams = getRequiredParameters();
        for (const auto& param : requiredParams) {
            if (params.find(param) == params.end()) {
                throw std::runtime_error("Missing required parameter: " + param);
            }
        }
        if (params.at("scale") <= 0) {
            throw std::runtime_error("Scale must be positive");
        }
        if (params.at("thickness") <= 0) {
            throw std::runtime_error("Thickness must be positive");
        }
    }

    Math::Point3D createCenter(const std::map<std::string, double>& params) const {
        Math::Coords coords {
            params.at("x"),
            params.at("y"),
            params.at("z")
        };
        return Math::Point3D(coords);
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new KleinBottlePlugin();
    }
}

}  // namespace RayTracer
