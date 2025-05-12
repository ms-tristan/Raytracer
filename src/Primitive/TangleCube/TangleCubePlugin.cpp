#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include "Primitive/Plugin/IPrimitivePlugin.hpp"
#include "Primitive/TangleCube/TangleCube.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class TangleCubePlugin : public IPrimitivePlugin {
 public:
    ~TangleCubePlugin() override = default;

    std::string getTypeName() const override {
        return TangleCube::getTypeNameStatic();
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
        double size = params.at("size");
        double spacing = params.at("spacing");

        return std::make_shared<TangleCube>(center, axis, size,
            spacing, material);
    }

    std::vector<std::string> getRequiredParameters() const override {
        return {"x", "y", "z", "ax", "ay", "az", "size", "spacing"};
    }
};

extern "C" {
    IPrimitivePlugin* createPrimitivePlugin() {
        return new TangleCubePlugin();
    }
}
}  // namespace RayTracer
