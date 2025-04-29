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
    std::string currentSceneFile;

 public:
    SceneDirector() = default;

    std::shared_ptr<Scene> createDefaultScene();
    std::shared_ptr<Scene> createSceneFromFile(const std::string& filename);
    std::shared_ptr<Scene> createBasicSphereScene();

    bool saveSceneToFile(const Scene& scene, const std::string& filename);

    bool saveScene(const Scene& scene);

    std::string getCurrentSceneFile() const { return currentSceneFile; }
};
}  // namespace RayTracer

#endif  // SRC_SCENE_SCENEDIRECTOR_SCENEDIRECTOR_HPP_
