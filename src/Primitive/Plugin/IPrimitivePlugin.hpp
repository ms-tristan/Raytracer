// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IPrimitivePlugin
*/

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

    virtual std::string getTypeName() const = 0;

    virtual std::shared_ptr<IPrimitive> createPrimitive(
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material) = 0;

    virtual std::vector<std::string> getRequiredParameters() const = 0;
};

extern "C" {
    IPrimitivePlugin* createPlugin();
}

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_PLUGIN_IPRIMITIVEPLUGIN_HPP_
