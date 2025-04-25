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

 private:
    void serializeCamera(const Camera& camera, libconfig::Setting& root);
    void serializePrimitives(const Scene& scene, libconfig::Setting& root);
    void serializeLights(const Scene& scene, libconfig::Setting& root);
    void serializePoint3D(const Math::Point3D& point,
                          libconfig::Setting& setting);
    void serializeVector3D(const Math::Vector3D& vector,
                           libconfig::Setting& setting);
    void serializeColor(const Math::Vector3D& color,
                        libconfig::Setting& setting);
};

}  // namespace RayTracer

#endif  // SRC_SCENE_SCENEDIRECTOR_SCENESERIALIZER_HPP_