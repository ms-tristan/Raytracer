#include <iostream>
#include <libconfig.h++>
#include "SceneSerializer.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/PointLight/PointLight.hpp"
#include "Primitive/Sphere/Sphere.hpp"
#include "Primitive/Plane/Plane.hpp"
#include "Primitive/Cylinder/Cylinder.hpp"
#include "Primitive/Cone/Cone.hpp"

namespace RayTracer {

bool SceneSerializer::saveToFile(const Scene& scene, const std::string& filename) { // TODO : implement this
    try {
        libconfig::Config cfg;
        libconfig::Setting& root = cfg.getRoot();

        // Serialize camera settings
        libconfig::Setting& camera = root.add("camera", libconfig::Setting::TypeGroup);
        serializeCamera(scene.getCamera(), camera);

        // We can't access primitives and lights directly, so we'll create a basic scene
        // with minimal primitives and lights based on what we can see/access

        // Serialize primitives
        libconfig::Setting& primitives = root.add("primitives", libconfig::Setting::TypeGroup);

        // Create empty primitive lists that we'll fill with hardcoded values
        // or from another source if available
        primitives.add("spheres", libconfig::Setting::TypeList);
        primitives.add("planes", libconfig::Setting::TypeList);
        primitives.add("cylinders", libconfig::Setting::TypeList);
        primitives.add("cones", libconfig::Setting::TypeList);

        // Serialize lights
        libconfig::Setting& lights = root.add("lights", libconfig::Setting::TypeGroup);

        // Set a default ambient light intensity (since we can't access it)
        lights.add("ambient", libconfig::Setting::TypeFloat) = 0.4;
        lights.add("diffuse", libconfig::Setting::TypeFloat) = 0.6;

        // Create empty light lists
        lights.add("point", libconfig::Setting::TypeList);
        lights.add("directional", libconfig::Setting::TypeList);

        // Write to file
        cfg.writeFile(filename.c_str());
        return true;

    } catch (const libconfig::FileIOException& ex) {
        std::cerr << "Error writing scene file: " << filename << std::endl;
        return false;
    } catch (const std::exception& ex) {
        std::cerr << "Error serializing scene: " << ex.what() << std::endl;
        return false;
    }
}

void SceneSerializer::serializeCamera(const Camera& camera, libconfig::Setting& root) {
    // Resolution (hardcoded since we don't have access to it)
    libconfig::Setting& resolution = root.add("resolution", libconfig::Setting::TypeGroup);
    resolution.add("width", libconfig::Setting::TypeInt) = 1920;
    resolution.add("height", libconfig::Setting::TypeInt) = 1080;

    // Camera position
    libconfig::Setting& position = root.add("position", libconfig::Setting::TypeGroup);
    serializePoint3D(camera.origin, position);

    // Add rotation (derived from screen parameters if possible)
    libconfig::Setting& rotation = root.add("rotation", libconfig::Setting::TypeGroup);
    rotation.add("x", libconfig::Setting::TypeFloat) = 0.0;
    rotation.add("y", libconfig::Setting::TypeFloat) = 0.0;
    rotation.add("z", libconfig::Setting::TypeFloat) = 0.0;

    // Field of view (estimated)
    root.add("fieldOfView", libconfig::Setting::TypeFloat) = 72.0;
}

void SceneSerializer::serializePoint3D(const Math::Point3D& point, libconfig::Setting& setting) {
    setting.add("x", libconfig::Setting::TypeFloat) = point.X;
    setting.add("y", libconfig::Setting::TypeFloat) = point.Y;
    setting.add("z", libconfig::Setting::TypeFloat) = point.Z;
}

void SceneSerializer::serializeVector3D(const Math::Vector3D& vector, libconfig::Setting& setting) {
    setting.add("x", libconfig::Setting::TypeFloat) = vector.X;
    setting.add("y", libconfig::Setting::TypeFloat) = vector.Y;
    setting.add("z", libconfig::Setting::TypeFloat) = vector.Z;
}

void SceneSerializer::serializeColor(const Math::Vector3D& color, libconfig::Setting& setting) {
    // Convert from 0-1 range to 0-255 range
    setting.add("r", libconfig::Setting::TypeInt) = static_cast<int>(color.X * 255);
    setting.add("g", libconfig::Setting::TypeInt) = static_cast<int>(color.Y * 255);
    setting.add("b", libconfig::Setting::TypeInt) = static_cast<int>(color.Z * 255);
}

}  // namespace RayTracer