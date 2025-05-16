/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ObjModelInfo
*/

#ifndef SRC_SCENE_SCENEDIRECTOR_OBJMODELINFO_HPP_
    #define SRC_SCENE_SCENEDIRECTOR_OBJMODELINFO_HPP_
    #include <string>
    #include <memory>
    #include "Material/Material.hpp"
    #include "Math/Vector3D/Vector3D.hpp"
    #include "defs.hpp"

namespace RayTracer {
struct ObjModelInfo {
    std::string path;
    Math::Vector3D position{Math::Coords{0, 0, 0}};
    Math::Vector3D rotation{Math::Coords{0, 0, 0}};
    double scale = 1.0;
    std::shared_ptr<Material> material;
};
}  // namespace RayTracer

#endif  // SRC_SCENE_SCENEDIRECTOR_OBJMODELINFO_HPP_
