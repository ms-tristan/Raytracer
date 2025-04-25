// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera
*/
#include "Camera/Camera.hpp"
#include "Transformation/Rotate/Rotate.hpp"

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
    RayTracer::Rotate rotateX("x", degrees);
    origin = rotateX.applyToPoint(origin);
    screen.origin = rotateX.applyToPoint(screen.origin);
    screen.bottom_side = rotateX.applyToVector(screen.bottom_side);
    screen.left_side = rotateX.applyToVector(screen.left_side);
}

void Camera::rotateY(double degrees) {
    RayTracer::Rotate rotateY("y", degrees);
    origin = rotateY.applyToPoint(origin);
    screen.origin = rotateY.applyToPoint(screen.origin);
    screen.bottom_side = rotateY.applyToVector(screen.bottom_side);
    screen.left_side = rotateY.applyToVector(screen.left_side);
}

void Camera::rotateZ(double degrees) {
    RayTracer::Rotate rotateZ("z", degrees);
    origin = rotateZ.applyToPoint(origin);
    screen.origin = rotateZ.applyToPoint(screen.origin);
    screen.bottom_side = rotateZ.applyToVector(screen.bottom_side);
    screen.left_side = rotateZ.applyToVector(screen.left_side);
}

}  // namespace RayTracer
