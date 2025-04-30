// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneBuilder
*/

#ifndef SRC_SCENE_SCENEBUILDER_SCENEBUILDER_HPP_
#define SRC_SCENE_SCENEBUILDER_SCENEBUILDER_HPP_
#include <memory>
#include <string>
#include <map>
#include "Scene/Scene.hpp"
#include "Camera/Camera.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Material/Material.hpp"
#include "Shader/IShader.hpp"
#include "PostProcess/IPostProcess.hpp"


namespace RayTracer {
class SceneBuilder {
 private:
    std::unique_ptr<Scene> scene;

 public:
    SceneBuilder();

    SceneBuilder& reset();
    SceneBuilder& setCamera(const Math::Point3D& origin,
        const Math::Point3D& lookAt);
    SceneBuilder& setCamera(const Camera& camera);
    SceneBuilder& setAmbientLight(const Math::Vector3D& color);

    SceneBuilder& addLight(const std::shared_ptr<ILight>& light);
    SceneBuilder& addShader(const std::shared_ptr<IShader>& shader);
    SceneBuilder& addPostProcess(const std::shared_ptr<IPostProcess>& postProcess);
    SceneBuilder& addPrimitive(const std::shared_ptr<IPrimitive>& primitive);

    SceneBuilder& createPrimitive(const std::string& type,
        const std::map<std::string, double>& params,
        const std::shared_ptr<Material>& material);

    SceneBuilder& addSphere(const Math::Point3D& center, double radius,
                          const std::shared_ptr<Material>& material);
    SceneBuilder& addPlane(const Math::Point3D& position,
        const Math::Vector3D& normal,
        const std::shared_ptr<Material>& material);
    SceneBuilder& addCylinder(const Math::Point3D& center,
        const Math::Vector3D& axis, double radius, double height,
        const std::shared_ptr<Material>& material);
    SceneBuilder& addCone(const Math::Point3D& apex,
        const Math::Vector3D& axis, double radius, double height,
        const std::shared_ptr<Material>& material);

    std::unique_ptr<Scene> build();
};
}  // namespace RayTracer

#endif  // SRC_SCENE_SCENEBUILDER_SCENEBUILDER_HPP_
