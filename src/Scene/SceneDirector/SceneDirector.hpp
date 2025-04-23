// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneDirector
*/

#ifndef SRC_SCENE_SCENEDIRECTOR_SCENEDIRECTOR_HPP_
    #define SRC_SCENE_SCENEDIRECTOR_SCENEDIRECTOR_HPP_
    #include <string>
    #include <memory>
    #include "Scene/SceneBuilder/SceneBuilder.hpp"

namespace RayTracer {
class SceneDirector {
 private:
    SceneBuilder builder;

 public:
    SceneDirector() = default;

    std::unique_ptr<Scene> createDefaultScene();
    std::unique_ptr<Scene> createSceneFromFile(const std::string& filename);
    std::unique_ptr<Scene> createBasicSphereScene();
};
}  // namespace RayTracer

#endif  // SRC_SCENE_SCENEDIRECTOR_SCENEDIRECTOR_HPP_
