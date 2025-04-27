// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera
*/
#include "Camera/Camera.hpp"
#include "Transformation/Rotation/Rotation.hpp"

namespace RayTracer {
Camera::Camera()
: origin(Math::Coords{0.0, 0.0, 0.0}),
screen(Math::Point3D(Math::Coords{-1.0, -1.0, -1.0}),
Math::Vector3D(Math::Coords{2.0, 0.0, 0.0}),
Math::Vector3D(Math::Coords{0.0, 2.0, 0.0})),
fov(90.0) {}

Camera::Camera(const Math::Point3D &origin,
const Rectangle3D &screen, double fov)
: origin(origin), screen(screen), fov(fov) {
    updateScreenForFOV();
}

void Camera::setFOV(double newFov) {
    fov = newFov;
    updateScreenForFOV();
}

double Camera::getFOV() const {
    return fov;
}

void Camera::updateScreenForFOV() {
    double distance = 1.0;
    double halfWidth = distance * std::tan(fov * M_PI / 360.0);

    Math::Point3D center = screen.origin + screen.bottom_side
        * 0.5 + screen.left_side * 0.5;

    double aspectRatio = screen.bottom_side.length() /
        screen.left_side.length();

    double newWidth = 2.0 * halfWidth;
    double newHeight = newWidth / aspectRatio;

    Math::Vector3D dirBottom = screen.bottom_side.normalize();
    Math::Vector3D dirLeft = screen.left_side.normalize();

    screen.bottom_side = dirBottom * newWidth;
    screen.left_side = dirLeft * newHeight;

    screen.origin = center - screen.bottom_side * 0.5 - screen.left_side * 0.5;
}

Ray Camera::ray(double u, double v) const {
    Math::Point3D point = screen.pointAt(u, v);
    Math::Vector3D direction = (point - origin).normalize();
    return Ray(origin, direction);
}

void Camera::translate(const Math::Vector3D &translation) { origin += translation; }

void Camera::rotateX(double degrees) {
    origin = Math::Rotation::rotateX(origin, degrees);
    screen.origin = Math::Rotation::rotateX(screen.origin, degrees);
    screen.bottom_side = Math::Rotation::rotateX(screen.bottom_side, degrees);
    screen.left_side = Math::Rotation::rotateX(screen.left_side, degrees);
}

void Camera::rotateY(double degrees) {
    origin = Math::Rotation::rotateY(origin, degrees);
    screen.origin = Math::Rotation::rotateY(screen.origin, degrees);
    screen.bottom_side = Math::Rotation::rotateY(screen.bottom_side, degrees);
    screen.left_side = Math::Rotation::rotateY(screen.left_side, degrees);
}

void Camera::rotateZ(double degrees) {
    origin = Math::Rotation::rotateZ(origin, degrees);
    screen.origin = Math::Rotation::rotateZ(screen.origin, degrees);
    screen.bottom_side = Math::Rotation::rotateZ(screen.bottom_side, degrees);
    screen.left_side = Math::Rotation::rotateZ(screen.left_side, degrees);
}

void Camera::getLibConfigParams(libconfig::Setting& setting) const {
    libconfig::Setting& resolution = setting.add("resolution", libconfig::Setting::TypeGroup);

    double width = screen.bottom_side.length();
    double height = screen.left_side.length();

    int screenWidth = static_cast<int>(std::round(width * 1000));
    int screenHeight = static_cast<int>(std::round(height * 1000));

    resolution.add("width", libconfig::Setting::TypeInt) = screenWidth;
    resolution.add("height", libconfig::Setting::TypeInt) = screenHeight;

    libconfig::Setting& position = setting.add("position", libconfig::Setting::TypeGroup);
    position.add("x", libconfig::Setting::TypeFloat) = origin.X;
    position.add("y", libconfig::Setting::TypeFloat) = origin.Y;
    position.add("z", libconfig::Setting::TypeFloat) = origin.Z;

    libconfig::Setting& rotation = setting.add("rotation", libconfig::Setting::TypeGroup);

    double xRotation = 0.0;
    double yRotation = 0.0;
    double zRotation = 0.0;

    rotation.add("x", libconfig::Setting::TypeFloat) = xRotation;
    rotation.add("y", libconfig::Setting::TypeFloat) = yRotation;
    rotation.add("z", libconfig::Setting::TypeFloat) = zRotation;

    setting.add("fieldOfView", libconfig::Setting::TypeFloat) = fov;
}

}  // namespace RayTracer
