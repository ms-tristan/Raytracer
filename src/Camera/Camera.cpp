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

}  // namespace RayTracer
