#ifndef SRC_PRIMITIVE_PLUGIN_IPRIMITIVEPLUGIN_HPP_
#define SRC_PRIMITIVE_PLUGIN_IPRIMITIVEPLUGIN_HPP_

#include <memory>
#include <vector>
#include <string>
#include <map>
#include "Primitive/IPrimitive.hpp"
#include "Material/Material.hpp"

namespace RayTracer {

class IPrimitivePlugin {
 public:
    virtual ~IPrimitivePlugin() = default;

    // Return a unique name for this primitive type
    virtual std::string getTypeName() const = 0;

    // Create an instance of the primitive
    virtual std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) = 0;

    // Returns a list of required parameters for this primitive type
    virtual std::vector<std::string> getRequiredParameters() const = 0;
};

// Define the entry point function that plugins must implement
extern "C" {
    IPrimitivePlugin* createPlugin();
}

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_PLUGIN_IPRIMITIVEPLUGIN_HPP_