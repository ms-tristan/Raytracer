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
    std::string currentSceneFile; // Track the currently loaded scene file

 public:
    SceneDirector() = default;

    std::unique_ptr<Scene> createDefaultScene();
    std::unique_ptr<Scene> createSceneFromFile(const std::string& filename);
    std::unique_ptr<Scene> createBasicSphereScene();
    
    // New method to save a scene to a file
    bool saveSceneToFile(const Scene& scene, const std::string& filename);
    
    // New method to save a scene to the file it was loaded from
    bool saveScene(const Scene& scene);
    
    // Get the current scene file path
    std::string getCurrentSceneFile() const { return currentSceneFile; }
};
}  // namespace RayTracer

#endif  // SRC_SCENE_SCENEDIRECTOR_SCENEDIRECTOR_HPP_
