// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneBuilder
*/
#include <utility>
#include <memory>
#include <map>
#include <iostream>
#include "SceneBuilder.hpp"
#include "Primitive/PrimitiveFactory/PrimitiveFactory.hpp"

namespace RayTracer {

SceneBuilder::SceneBuilder() : scene(std::make_unique<Scene>()) {
}

SceneBuilder& SceneBuilder::reset() {
    scene = std::make_unique<Scene>();
    return *this;
}

SceneBuilder& SceneBuilder::setCamera(const Math::Point3D& origin,
const Math::Point3D& lookAt) {
    Math::Vector3D forward = (lookAt - origin).normalize();
    Math::Vector3D up(Math::Coords{0.0, 1.0, 0.0});
    Math::Vector3D right = up.cross(forward).normalize();

    up = forward.cross(right).normalize();

    double distance = 1.0;
    double width = 2.0;
    double height = 2.0;

    Math::Point3D screenCenter = origin + (forward * distance);
    Rectangle3D screen(
        screenCenter - (right * (width/2)) - (up * (height/2)),
        right * width,
        up * height);

    Camera camera(origin, screen);
    scene->setCamera(camera);
    return *this;
}

SceneBuilder& SceneBuilder::setAmbientLight(const Math::Vector3D& color) {
    AmbientLight light(color);
    scene->setAmbientLight(light);
    return *this;
}

SceneBuilder& SceneBuilder::addLight(const std::shared_ptr<ILight>& light) {
    scene->addLight(light);
    return *this;
}

SceneBuilder& SceneBuilder::addPrimitive(const
std::shared_ptr<IPrimitive>& primitive) {
    scene->addPrimitive(primitive);
    return *this;
}

SceneBuilder& SceneBuilder::addSphere(const Math::Point3D& center,
double radius, const std::shared_ptr<Material>& material) {
    std::map<std::string, double> params = {
        {"x", center.X},
        {"y", center.Y},
        {"z", center.Z},
        {"radius", radius}
    };

    try {
        auto sphere = PrimitiveFactory::createPrimitive("sphere", params, material);
        scene->addPrimitive(sphere);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create sphere: " << e.what() << std::endl;
    }
    return *this;
}

SceneBuilder& SceneBuilder::addPlane(const Math::Point3D& position,
const Math::Vector3D& normal, const std::shared_ptr<Material>& material) {
    std::map<std::string, double> params = {
        {"x", position.X},
        {"y", position.Y},
        {"z", position.Z},
        {"nx", normal.X},
        {"ny", normal.Y},
        {"nz", normal.Z}
    };

    try {
        auto plane = PrimitiveFactory::createPrimitive("plane", params, material);
        scene->addPrimitive(plane);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create plane: " << e.what() << std::endl;
    }
    return *this;
}

SceneBuilder& SceneBuilder::addCylinder(const Math::Point3D& center,
const Math::Vector3D& axis, double radius, double height,
const std::shared_ptr<Material>& material) {
    std::map<std::string, double> params = {
        {"x", center.X},
        {"y", center.Y},
        {"z", center.Z},
        {"ax", axis.X},
        {"ay", axis.Y},
        {"az", axis.Z},
        {"radius", radius},
        {"height", height}
    };

    try {
        auto cylinder = PrimitiveFactory::createPrimitive("cylinder", params, material);
        scene->addPrimitive(cylinder);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create cylinder: " << e.what() << std::endl;
    }
    return *this;
}

SceneBuilder& SceneBuilder::addCone(const Math::Point3D& apex,
const Math::Vector3D& axis, double radius, double height,
const std::shared_ptr<Material>& material) {
    std::map<std::string, double> params = {
        {"x", apex.X},
        {"y", apex.Y},
        {"z", apex.Z},
        {"ax", axis.X},
        {"ay", axis.Y},
        {"az", axis.Z},
        {"radius", radius},
        {"height", height}
    };

    try {
        auto cone = PrimitiveFactory::createPrimitive("cone", params, material);
        scene->addPrimitive(cone);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create cone: " << e.what() << std::endl;
    }
    return *this;
}

std::unique_ptr<Scene> SceneBuilder::build() {
    return std::move(scene);
}

}  // namespace RayTracer
