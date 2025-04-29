// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** LightFactory
*/

#ifndef SRC_LIGHT_LIGHTFACTORY_LIGHTFACTORY_HPP_
    #define SRC_LIGHT_LIGHTFACTORY_LIGHTFACTORY_HPP_
    #include <functional>
    #include <map>
    #include <memory>
    #include <string>
    #include <vector>
    #include "Light/ILight.hpp"
    #include "Light/AmbientLight/AmbientLight.hpp"
    #include "Math/Vector3D/Vector3D.hpp"
    #include "Math/Point3D/Point3D.hpp"

namespace RayTracer {

class LightFactory {
 private:
    using CreatorFunc = std::function<std::shared_ptr<ILight>(
        const std::map<std::string, double>&)>;
    static std::map<std::string, CreatorFunc> creators;
    static bool isInitialized;

    static void registerBuiltInLights();

 public:
    static std::shared_ptr<ILight> createLight(
        const std::string& type,
        const std::map<std::string, double>& params);

    static void registerLight(const std::string& type, CreatorFunc creator);

    static std::vector<std::string> getAvailableLightTypes();

    static AmbientLight createAmbientLight(const Math::Vector3D& color);
};

}  // namespace RayTracer

#endif  // SRC_LIGHT_LIGHTFACTORY_LIGHTFACTORY_HPP_
