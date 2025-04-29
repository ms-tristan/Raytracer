// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneReloader
*/

#ifndef SRC_IOBSERVER_SCENEFILEWATCHER_SCENERELOADER_HPP_
#define SRC_IOBSERVER_SCENEFILEWATCHER_SCENERELOADER_HPP_
#include <memory>
#include <string>
#include "IObserver/IObserver.hpp"
#include "Scene/SceneDirector/SceneDirector.hpp"
#include "Scene/Scene.hpp"

namespace RayTracer {
class SceneReloader : public IObserver {
 private:
    std::string filePath;
    SceneDirector director;
    std::shared_ptr<Scene> scene;

 public:
    explicit SceneReloader(const std::string& filePath);
    ~SceneReloader() override = default;

    void update() override;
    std::shared_ptr<Scene> getScene() const;
};
}  // namespace RayTracer

#endif  // SRC_IOBSERVER_SCENEFILEWATCHER_SCENERELOADER_HPP_
