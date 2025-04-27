#ifndef SRC_SCENE_SCENEDIRECTOR_SCENESERIALIZER_HPP_
#define SRC_SCENE_SCENEDIRECTOR_SCENESERIALIZER_HPP_

#include <string>
#include <libconfig.h++>
#include "Scene/Scene.hpp"
#include "Camera/Camera.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class SceneSerializer {
 public:
    bool saveToFile(const Scene& scene, const std::string& filename);
};

}  // namespace RayTracer

#endif  // SRC_SCENE_SCENEDIRECTOR_SCENESERIALIZER_HPP_