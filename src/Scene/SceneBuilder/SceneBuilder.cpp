// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneBuilder
*/
#include <string>
#include <utility>
#include <memory>
#include <map>
#include <iostream>
#include "SceneBuilder.hpp"
#include "Primitive/PrimitiveFactory/PrimitiveFactory.hpp"
#include "Primitive/Plugin/PluginLoader.hpp"

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

SceneBuilder& SceneBuilder::setCamera(const Camera& camera) {
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

SceneBuilder& SceneBuilder::addShader(const std::shared_ptr<IShader>& shader) {
    scene->addShader(shader);
    return *this;
}

SceneBuilder& SceneBuilder::addPrimitive(const
std::shared_ptr<IPrimitive>& primitive) {
    scene->addPrimitive(primitive);
    return *this;
}

SceneBuilder& SceneBuilder::createPrimitive(const std::string& type,
const std::map<std::string, double>& params,
const std::shared_ptr<Material>& material) {
    try {
        auto primitive = PrimitiveFactory::createPrimitive(type, params, material);
        scene->addPrimitive(primitive);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create primitive of type " << type << ": " << e.what() << std::endl;
    }
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

    return createPrimitive("spheres", params, material);
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

    return createPrimitive("planes", params, material);
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

    return createPrimitive("cylinders", params, material);
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

    return createPrimitive("cones", params, material);
}

SceneBuilder& SceneBuilder::addTorus(const Math::Point3D& center,
const Math::Vector3D& axis, double majorRadius, double minorRadius,
const std::shared_ptr<Material>& material) {
    std::map<std::string, double> params = {
        {"x", center.X},
        {"y", center.Y},
        {"z", center.Z},
        {"ax", axis.X},
        {"ay", axis.Y},
        {"az", axis.Z},
        {"majorRadius", majorRadius},
        {"minorRadius", minorRadius}
    };

    return createPrimitive("torus", params, material);
}

SceneBuilder& SceneBuilder::addPostProcess(const std::shared_ptr<IPostProcess>& postProcess) {
    scene->addPostProcess(postProcess);
    return *this;
}

std::unique_ptr<Scene> SceneBuilder::build() {
    return std::move(scene);
}

}  // namespace RayTracer
