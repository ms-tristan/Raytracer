// // Copyright <2025> Epitech
// /*
// ** EPITECH PROJECT, 2025
// ** Raytracer
// ** File description:
// ** Main with demonstration of all features including plugins and decorators
// */

// #include <iostream>
// #include <memory>
// #include <algorithm>
// #include <string>
// #include <thread>
// #include <chrono>
// #include <map>
// #include "Scene/Scene.hpp"
// #include "Scene/SceneBuilder/SceneBuilder.hpp"
// #include "Scene/SceneDirector/SceneDirector.hpp"
// #include "Camera/Camera.hpp"
// #include "Rectangle3D/Rectangle3D.hpp"
// #include "Primitive/CompositePrimitive/CompositePrimitive.hpp"
// #include "Primitive/PrimitiveDecorator/PrimitiveDecorator.hpp"
// #include "Primitive/Plugin/PrimitivePluginManager.hpp"
// #include "Primitive/Plugin/PluginLoader.hpp"
// #include "Material/Material.hpp"
// #include "Material/MaterialDecorator/MaterialDecorator.hpp"
// #include "Light/DirectionalLight/DirectionalLight.hpp"
// #include "Light/AmbientLight/AmbientLight.hpp"
// #include "Light/PointLight/PointLight.hpp"
// #include "Light/LightDecorator/LightDecorator.hpp"
// #include "Math/Vector3D/Vector3D.hpp"
// #include "Math/Point3D/Point3D.hpp"
// #include "Ray/Ray.hpp"
// #include "IObserver/SceneFileWatcher/SceneFileWatcher.hpp"
// #include "IObserver/SceneFileWatcher/SceneReloader.hpp"

// class RotatingDecorator : public RayTracer::PrimitiveDecorator {
//  public:
//     RotatingDecorator(std::shared_ptr<RayTracer::IPrimitive> primitive,
//         double degrees, const Math::Vector3D& axis)
//         : PrimitiveDecorator(primitive), degrees(degrees),
//             axis(axis), currentAngle(0) {}

//     std::optional<RayTracer::HitInfo> hit(const RayTracer::Ray &ray,
//         double tMin, double tMax) const override {
//         auto result = wrappedPrimitive->hit(ray, tMin, tMax);
//         return result;
//     }

//     void update() {
//         currentAngle += degrees;
//         if (currentAngle >= 360.0) {
//             currentAngle -= 360.0;
//         }

//         if (axis.X > 0.5) wrappedPrimitive->rotateX(degrees);
//         else if (axis.Y > 0.5) wrappedPrimitive->rotateY(degrees);
//         else wrappedPrimitive->rotateZ(degrees);
//     }

//     std::shared_ptr<RayTracer::IPrimitive> clone() const override {
//         return std::make_shared<RotatingDecorator>(
//             wrappedPrimitive->clone(), degrees, axis);
//     }

//  private:
//     double degrees;
//     Math::Vector3D axis;
//     double currentAngle;
// };

// int main(int argc, char **argv) {

//     const int image_width = 800;
//     const int image_height = 600;

//     std::cout << "P3" << std::endl;
//     std::cout << image_width << " " << image_height << std::endl;
//     std::cout << "255" << std::endl;

//     auto redMaterial = std::make_shared<RayTracer::Material>();
//     redMaterial->color = Math::Vector3D(Math::Coords{0.9, 0.2, 0.2});

//     auto blueMaterial = std::make_shared<RayTracer::Material>();
//     blueMaterial->color = Math::Vector3D(Math::Coords{0.0, 0.1, 0.9});

//     auto greenMaterial = std::make_shared<RayTracer::Material>();
//     greenMaterial->color = Math::Vector3D(Math::Coords{0.1, 0.8, 0.2});

//     auto planeMaterial = std::make_shared<RayTracer::Material>();
//     planeMaterial->color = Math::Vector3D(Math::Coords{0.396, 0.502, 0.651});

//     auto yellowMaterial = std::make_shared<RayTracer::Material>();
//     yellowMaterial->color = Math::Vector3D(Math::Coords{0.9, 0.8, 0.2});

//     RayTracer::SceneBuilder builder;

//     builder.setCamera(
//         Math::Point3D(Math::Coords{0.0, 1.0, 2.5}),
//         Math::Point3D(Math::Coords{0.0, 0.0, -1.0}));

//     builder.setAmbientLight(Math::Vector3D(Math::Coords{0.15, 0.15, 0.18}));

//     auto mainLight = std::make_shared<RayTracer::DirectionalLight>(
//         Math::Vector3D(Math::Coords{0.3, -1.0, -0.5}),
//         Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}));
//     builder.addLight(mainLight);

//     auto fillLight = std::make_shared<RayTracer::DirectionalLight>(
//         Math::Vector3D(Math::Coords{-0.6, -0.4, -0.2}),
//         Math::Vector3D(Math::Coords{0.5, 0.5, 0.6}));
//     builder.addLight(fillLight);

//     auto pointLight = std::make_shared<RayTracer::PointLight>(
//         Math::Point3D(Math::Coords{1.0, 1.0, 0.0}),
//         Math::Vector3D(Math::Coords{0.8, 0.8, 1.0}),
//         1.0,
//         0.09,
//         0.032);
//     builder.addLight(pointLight);

//     auto pluginManager = RayTracer::PrimitivePluginManager::getInstance();

//     RayTracer::loadPrimitivePlugins();

//     auto availablePlugins = pluginManager->getLoadedPluginNames();
//     if (availablePlugins.empty()) {
//         std::cerr << "Warning: No plugins were loaded" << std::endl;
//         return 1;
//     }

//     if (pluginManager->getPlugin("sphere")) {
//         std::map<std::string, double> sphereParams = {
//             {"x", -0.9}, {"y", 0.5}, {"z", -1.3}, {"radius", 0.4}
//         };

//         auto spherePrimitive = pluginManager->createPrimitive("sphere",
//             sphereParams, redMaterial);

//         if (spherePrimitive)
//             builder.addPrimitive(spherePrimitive);
//         else
//             std::cerr << "Failed to create sphere from plugin" << std::endl;
//     }

//     if (pluginManager->getPlugin("box")) {
//         std::map<std::string, double> boxParams = {
//             {"x", 0.5}, {"y", 0.2}, {"z", -1.2},
//             {"width", 0.4}, {"height", 0.3}, {"depth", 0.4}
//         };

//         auto boxPrimitive = pluginManager->createPrimitive(
//             "box", boxParams, greenMaterial);

//         if (boxPrimitive)
//             builder.addPrimitive(boxPrimitive);
//         else
//             std::cerr << "Failed to create box from plugin" << std::endl;
//     }

//     if (pluginManager->getPlugin("cone")) {
//         std::map<std::string, double> coneParams = {
//             {"x", -0.4}, {"y", 0.3}, {"z", -1.5},
//             {"ax", 0}, {"ay", -1}, {"az", 0},
//             {"radius", 0.4}, {"height", 0.8}
//         };

//         auto conePrimitive = pluginManager->createPrimitive(
//             "cone", coneParams, blueMaterial);

//         if (conePrimitive) {
//             auto compositeMaterial = std::make_shared<RayTracer::Material>();
//             compositeMaterial->color = Math::Vector3D(Math::Coords{0.5,
//                 0.5, 0.5});
//             auto compositeGroup = std::make_shared<
//                 RayTracer::CompositePrimitive>(compositeMaterial);

//             compositeGroup->add(conePrimitive);

//             if (pluginManager->getPlugin("sphere")) {
//                 std::map<std::string, double> smallSphereParams = {
//                     {"x", -0.1}, {"y", -0.1}, {"z", -1.0},
//                     {"radius", 0.15}
//                 };

//                 auto smallSpherePrimitive = pluginManager->createPrimitive(
//                     "sphere", smallSphereParams, yellowMaterial);

//                 if (smallSpherePrimitive) {
//                     compositeGroup->add(smallSpherePrimitive);
//                 }
//             }

//             builder.addPrimitive(compositeGroup);
//         } else {
//             std::cerr << "Failed to create cone from plugin" << std::endl;
//         }
//     }

//     if (pluginManager->getPlugin("plane")) {
//         std::map<std::string, double> planeParams = {
//             {"x", 0}, {"y", -0.7}, {"z", 0},
//             {"nx", 0}, {"ny", 1}, {"nz", 0}
//         };

//         auto planePrimitive = pluginManager->createPrimitive(
//             "plane", planeParams, planeMaterial);

//         if (planePrimitive)
//             builder.addPrimitive(planePrimitive);
//         else
//             std::cerr << "Failed to create plane from plugin" << std::endl;
//     }

//     std::unique_ptr<RayTracer::Scene> scene = builder.build();

//     RayTracer::Camera camera = scene->getCamera();

//     std::shared_ptr<RayTracer::CompositePrimitive> compositeGroup;
//     std::shared_ptr<RotatingDecorator> rotatingCone;

//     int frameCount = 1;
//     for (int frame = 0; frame < frameCount; frame++) {
//         for (const auto& primitive : scene->getPrimitives()) {
//             auto composite = std::dynamic_pointer_cast<
//                 RayTracer::CompositePrimitive>(primitive);
//             if (composite) {
//                 for (const auto& subPrimitive : composite->getPrimitives()) {
//                     auto rotatingDecorator = std::dynamic_pointer_cast<
//                         RotatingDecorator>(subPrimitive);
//                     if (rotatingDecorator)
//                         rotatingDecorator->update();
//                 }
//             }
//         }

//         for (int j = image_height - 1; j >= 0; --j) {
//             for (int i = 0; i < image_width; ++i) {
//                 double u = static_cast<double>(i) / (image_width - 1);
//                 double v = static_cast<double>(j) / (image_height - 1);

//                 RayTracer::Ray ray = camera.ray(u, v);
//                 Math::Vector3D pixelColor = scene->computeColor(ray);

//                 scene->writeColor(pixelColor);
//             }
//         }
//     }

//     return 0;
// }

// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Main with demonstration of design patterns
*/

#include <iostream>
#include <memory>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include <SFML/Graphics.hpp>

// Display Manager
#include "./Renderer/DisplayManager/SFMLDisplayManager.hpp"
#include "./Renderer/Renderer.hpp"

// Events Manager
#include "./EventsManager/SFMLEventsManager.hpp"
#include "./EventsManager/InputManager/InputManager.hpp"

// Scene components
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder/SceneBuilder.hpp"
#include "Scene/SceneDirector/SceneDirector.hpp"
#include "Camera/Camera.hpp"
#include "Rectangle3D/Rectangle3D.hpp"

// Primitives
#include "Primitive/CompositePrimitive/CompositePrimitive.hpp"

// Lights and Materials
#include "Material/Material.hpp"
#include "Light/DirectionalLight/DirectionalLight.hpp"
#include "Light/AmbientLight/AmbientLight.hpp"
#include "Light/PointLight/PointLight.hpp"

// Math utilities
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Ray/Ray.hpp"


int main(int argc, char **argv) {
    const int image_width = 800;
    const int image_height = 600;

    RayTracer::SceneDirector director;
    std::unique_ptr<RayTracer::Scene> scene;
    std::string sceneFile = "scenes/default_scene.cfg";

    try {
        if (argc > 1) {
            sceneFile = argv[1];
            std::cout << "Loading scene from file: " << sceneFile << std::endl;
            scene = director.createSceneFromFile(sceneFile);
        } else {
            std::cout << "No scene file specified, using default scene." << std::endl;
            scene = director.createDefaultScene();
        }

        RayTracer::Camera camera = scene->getCamera();

        RayTracer::SFMLDisplayManager displayManager;

        RayTracer::SFMLEventsManager eventsManager(displayManager.getWindow());

        displayManager.initialize(image_width, image_height, "Raytracer", false);

        RayTracer::Renderer renderer(displayManager);

        RayTracer::InputManager inputManager(eventsManager, image_width, image_height);

        auto prims = scene->getPrimitives();

        std::cout << "Loaded " << prims.size() << " primitives." << std::endl;

        for (const auto& primitive : prims) {
            std::cout << "Primitive: " << primitive->getTypeName() << std::endl;
        }


        while (displayManager.isWindowOpen()) {

            renderer.drawScene(*scene, camera);
            inputManager.processInput(*scene, camera);

            if (eventsManager.isKeyPressed("ESCAPE")) {
                displayManager.closeWindow();
            }
        }

        scene->setCamera(camera);

        if (!director.saveSceneToFile(*scene, sceneFile)) {
            std::cerr << "Failed to save the scene to " << sceneFile << std::endl;
        } else {
            std::cout << "Scene saved successfully to " << sceneFile << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
}
