// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scene
*/

#ifndef SRC_SCENE_SCENE_HPP_
  #define SRC_SCENE_SCENE_HPP_
  #include <algorithm>
  #include <limits>
  #include <memory>
  #include <vector>
  #include "Camera/Camera.hpp"
  #include "HitInfo.hpp"
  #include "Light/AmbientLight/AmbientLight.hpp"
  #include "Light/ILight.hpp"
  #include "Primitive/IPrimitive.hpp"
  #include "Ray/Ray.hpp"

namespace RayTracer {
class Scene {
 private:
    std::vector<std::shared_ptr<IPrimitive>> primitives;
    std::vector<std::shared_ptr<ILight>> lights;
    AmbientLight ambientLight;
    Camera camera;

 public:
    Scene() = default;

    void setCamera(const Camera &cam);
    Camera getCamera() const;
    void setAmbientLight(const AmbientLight &light);
    void addPrimitive(const std::shared_ptr<IPrimitive> &primitive);
    void addLight(const std::shared_ptr<ILight> &light);
    std::optional<HitInfo> trace(const Ray &ray) const;
    bool isInShadow(const Math::Point3D &hitPoint,
      const Math::Vector3D &lightDir,
      const std::shared_ptr<ILight> &light) const;
    Math::Vector3D computeColor(const Ray &ray) const;
    void writeColor(const Math::Vector3D &color);
};
}  // namespace RayTracer

#endif  // SRC_SCENE_SCENE_HPP_
