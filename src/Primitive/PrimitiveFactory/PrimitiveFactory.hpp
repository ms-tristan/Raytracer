// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PrimitiveFactory
*/

#ifndef SRC_PRIMITIVE_PRIMITIVEFACTORY_PRIMITIVEFACTORY_HPP_
#define SRC_PRIMITIVE_PRIMITIVEFACTORY_PRIMITIVEFACTORY_HPP_
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "Primitive/IPrimitive.hpp"
#include "Material/Material.hpp"

namespace RayTracer {
class PrimitiveFactory {
 private:
    using CreatorFunc = std::function<std::shared_ptr<IPrimitive>(
        const std::map<std::string, double>&,
        const std::shared_ptr<Material>&)>;
    static std::map<std::string, CreatorFunc> creators;

    static bool isInitialized;

 public:
    static std::shared_ptr<IPrimitive> createPrimitive(
        const std::string& type,
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material);

    static void registerPrimitive(const std::string& type, CreatorFunc creator);

    static std::vector<std::string> getAvailablePrimitiveTypes();
};
}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_PRIMITIVEFACTORY_PRIMITIVEFACTORY_HPP_
