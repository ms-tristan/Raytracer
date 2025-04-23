// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SceneBuilder
*/
#include <utility>
#include <memory>
#include "SceneBuilder.hpp"
#include "Primitive/Sphere/Sphere.hpp"
#include "Primitive/Plane/Plane.hpp"
#include "Primitive/Cylinder/Cylinder.hpp"
#include "Primitive/Cone/Cone.hpp"

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
    auto sphere = std::make_shared<Sphere>(center, radius, material);
    scene->addPrimitive(sphere);
    return *this;
}

SceneBuilder& SceneBuilder::addPlane(const Math::Point3D& position,
const Math::Vector3D& normal, const std::shared_ptr<Material>& material) {
    auto plane = std::make_shared<Plane>(position, normal, material);
    scene->addPrimitive(plane);
    return *this;
}

SceneBuilder& SceneBuilder::addCylinder(const Math::Point3D& center,
const Math::Vector3D& axis, double radius, double height,
const std::shared_ptr<Material>& material) {
    auto cylinder = std::make_shared<Cylinder>(center, axis,
        radius, height, material);
    scene->addPrimitive(cylinder);
    return *this;
}

SceneBuilder& SceneBuilder::addCone(const Math::Point3D& apex,
const Math::Vector3D& axis, double radius, double height,
const std::shared_ptr<Material>& material) {
    auto cone = std::make_shared<Cone>(apex, axis, radius, height, material);
    scene->addPrimitive(cone);
    return *this;
}

std::unique_ptr<Scene> SceneBuilder::build() {
    return std::move(scene);
}

}  // namespace RayTracer
