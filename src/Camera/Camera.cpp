// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera
*/
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Camera/Camera.hpp"
#include "Transformation/Rotate/Rotate.hpp"
#include "Scene/Scene.hpp"

namespace RayTracer {
Camera::Camera()
: origin(Math::Coords{0.0, 0.0, 0.0}),
screen(Math::Point3D(Math::Coords{-1.0, -1.0, -1.0}),
Math::Vector3D(Math::Coords{2.0, 0.0, 0.0}),
Math::Vector3D(Math::Coords{0.0, 2.0, 0.0})),
fov(90.0),
originalRotation(Math::Coords{0.0, 0.0, 0.0}),
rotatedX(0.0),
rotatedY(0.0),
rotatedZ(0.0) {}

Camera::Camera(const Math::Point3D &origin,
const Rectangle3D &screen, double fov)
: origin(origin), screen(screen), fov(fov),
originalRotation(Math::Coords{0.0, 0.0, 0.0}),
rotatedX(0.0),
rotatedY(0.0),
rotatedZ(0.0) {
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

Math::Vector3D Camera::supersampleRay(double u, double v, const Scene& scene, int samplesPerPixel) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(-0.5, 0.5);
    int samplesPerAxis = static_cast<int>(std::sqrt(samplesPerPixel));
    double pixelSizeU = 1.0 / std::max(1, scene.getImageWidth() - 1);
    double pixelSizeV = 1.0 / std::max(1, scene.getImageHeight() - 1);
    Math::Vector3D accumulatedColor(Math::Coords{0.0, 0.0, 0.0});
    if (samplesPerAxis > 1) {
        for (int i = 0; i < samplesPerAxis; ++i) {
            for (int j = 0; j < samplesPerAxis; ++j) {
                double offsetU = (i + 0.5) / samplesPerAxis - 0.5;
                double offsetV = (j + 0.5) / samplesPerAxis - 0.5;
                double sampleU = u + offsetU * pixelSizeU;
                double sampleV = v + offsetV * pixelSizeV;
                Ray sampleRay = ray(sampleU, sampleV);
                Math::Vector3D sampleColor = scene.computeColor(sampleRay);
                accumulatedColor += sampleColor;
            }
        }
    } else {
        for (int i = 0; i < samplesPerPixel; ++i) {
            double offsetU = dis(gen);
            double offsetV = dis(gen);
            double sampleU = u + offsetU * pixelSizeU;
            double sampleV = v + offsetV * pixelSizeV;
            Ray sampleRay = ray(sampleU, sampleV);
            Math::Vector3D sampleColor = scene.computeColor(sampleRay);
            accumulatedColor += sampleColor;
        }
    }
    accumulatedColor.X /= samplesPerPixel;
    accumulatedColor.Y /= samplesPerPixel;
    accumulatedColor.Z /= samplesPerPixel;
    return accumulatedColor;
}

void Camera::translate(const Math::Vector3D &translation) {
    origin += translation;
    screen.origin += translation;
}

void Camera::rotateX(double degrees) {
    rotatedX += degrees;
    Math::Point3D screenCenter = screen.origin +
        screen.bottom_side * 0.5 + screen.left_side * 0.5;
    Math::Vector3D viewDir = (screenCenter - origin).normalize();
    Math::Vector3D rightDir = screen.bottom_side.normalize();
    Math::Vector3D upDir = screen.left_side.normalize();
    double screenDistance = (screenCenter - origin).length();
    double screenWidth = screen.bottom_side.length();
    double screenHeight = screen.left_side.length();

    double angleRad = RayTracer::toRadians(degrees);
    double cosAngle = cos(angleRad);
    double sinAngle = sin(angleRad);

    Math::Vector3D newViewDir = viewDir * cosAngle +
                          rightDir.cross(viewDir) * sinAngle +
                          rightDir * rightDir.dot(viewDir) * (1 - cosAngle);
    newViewDir = newViewDir.normalize();

    Math::Vector3D newUpDir = rightDir.cross(newViewDir).normalize();

    Math::Point3D newScreenCenter = origin + newViewDir * screenDistance;
    Math::Vector3D halfWidthVec = rightDir * (screenWidth/2);
    Math::Vector3D halfHeightVec = newUpDir * (screenHeight/2);

    screen.origin = newScreenCenter - halfWidthVec - halfHeightVec;
    screen.bottom_side = rightDir * screenWidth;
    screen.left_side = newUpDir * screenHeight;
}

void Camera::rotateY(double degrees) {
    rotatedY += degrees;
    Math::Point3D screenCenter = screen.origin +
        screen.bottom_side * 0.5 + screen.left_side * 0.5;
    Math::Vector3D viewDir = (screenCenter - origin).normalize();
    double screenDistance = (screenCenter - origin).length();
    double screenWidth = screen.bottom_side.length();
    double screenHeight = screen.left_side.length();

    Math::Vector3D newViewDir = Rotate("y", degrees).applyToVector(viewDir);
    newViewDir = newViewDir.normalize();

    Math::Vector3D globalUp(Math::Coords{0.0, 1.0, 0.0});
    Math::Vector3D rightDir = newViewDir.cross(globalUp).normalize();

    if (rightDir.length() < 0.001) {
        Math::Vector3D tempRef(Math::Coords{1.0, 0.0, 0.0});
        rightDir = newViewDir.cross(tempRef).normalize();
    }

    Math::Vector3D upDir = rightDir.cross(newViewDir).normalize();

    Math::Point3D newScreenCenter = origin + newViewDir * screenDistance;
    Math::Vector3D halfWidthVec = rightDir * (screenWidth/2);
    Math::Vector3D halfHeightVec = upDir * (screenHeight/2);

    screen.origin = newScreenCenter - halfWidthVec - halfHeightVec;
    screen.bottom_side = rightDir * screenWidth;
    screen.left_side = upDir * screenHeight;
}

void Camera::rotateZ(double degrees) {
    rotatedZ += degrees;
    Math::Point3D screenCenter = screen.origin +
        screen.bottom_side * 0.5 + screen.left_side * 0.5;
    Math::Vector3D viewDir = (screenCenter - origin).normalize();
    double screenDistance = (screenCenter - origin).length();
    double screenWidth = screen.bottom_side.length();
    double screenHeight = screen.left_side.length();

    Math::Vector3D rightDir = Rotate("z", degrees).applyToVector(screen.bottom_side.normalize());
    Math::Vector3D upDir = Rotate("z", degrees).applyToVector(screen.left_side.normalize());

    rightDir = rightDir.normalize();
    upDir = upDir.normalize();

    Math::Point3D newScreenCenter = origin + viewDir * screenDistance;
    Math::Vector3D halfWidthVec = rightDir * (screenWidth/2);
    Math::Vector3D halfHeightVec = upDir * (screenHeight/2);

    screen.origin = newScreenCenter - halfWidthVec - halfHeightVec;
    screen.bottom_side = rightDir * screenWidth;
    screen.left_side = upDir * screenHeight;
}

Math::Vector3D Camera::calculateRotationAngles() const {
    Math::Point3D screenCenter = screen.origin +
        screen.bottom_side * 0.5 + screen.left_side * 0.5;
    Math::Vector3D viewDir = (screenCenter - origin).normalize();
    Math::Vector3D rightDir = screen.bottom_side.normalize();
    Math::Vector3D upDir = screen.left_side.normalize();

    double yRotation = atan2(viewDir.X, viewDir.Z) * 180.0 / M_PI;

    double xRotation = -asin(viewDir.Y) * 180.0 / M_PI;

    Math::Vector3D initialViewDir(Math::Coords{0.0, 0.0, -1.0});
    Math::Vector3D initialUpDir(Math::Coords{0.0, 1.0, 0.0});
    Math::Vector3D initialRightDir(Math::Coords{1.0, 0.0, 0.0});

    Math::Vector3D rotatedViewDir = Rotate("y", yRotation).applyToVector(initialViewDir);
    Math::Vector3D rotatedUpDir = Rotate("y", yRotation).applyToVector(initialUpDir);
    Math::Vector3D rotatedRightDir = Rotate("y", yRotation).applyToVector(initialRightDir);

    rotatedViewDir = Rotate("x", xRotation).applyToVector(rotatedViewDir);
    rotatedUpDir = Rotate("x", xRotation).applyToVector(rotatedUpDir);
    rotatedRightDir = Rotate("x", xRotation).applyToVector(rotatedRightDir);

    double dotProduct = upDir.dot(rotatedUpDir);
    double crossLength = upDir.cross(rotatedUpDir).length();
    double zRotation = atan2(crossLength, dotProduct) * 180.0 / M_PI;

    Math::Vector3D crossResult = upDir.cross(rotatedUpDir);
    if (crossResult.dot(viewDir) < 0) {
        zRotation = -zRotation;
    }

    return Math::Vector3D(Math::Coords{xRotation, yRotation, zRotation});
}

void Camera::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& resolution = setting->add("resolution", libconfig::Setting::TypeGroup);

    double width = screen.bottom_side.length();
    double height = screen.left_side.length();

    int screenWidth = static_cast<int>(std::round(width * 1000));
    int screenHeight = static_cast<int>(std::round(height * 1000));

    resolution.add("width", libconfig::Setting::TypeInt) = screenWidth;
    resolution.add("height", libconfig::Setting::TypeInt) = screenHeight;

    libconfig::Setting& position = setting->add("position", libconfig::Setting::TypeGroup);
    position.add("x", libconfig::Setting::TypeFloat) = origin.X;
    position.add("y", libconfig::Setting::TypeFloat) = origin.Y;
    position.add("z", libconfig::Setting::TypeFloat) = origin.Z;

    libconfig::Setting& rotation = setting->add("rotation", libconfig::Setting::TypeGroup);

    double totalRotationX = originalRotation.X + rotatedX;
    double totalRotationY = originalRotation.Y + rotatedY;
    double totalRotationZ = originalRotation.Z + rotatedZ;

    rotation.add("x", libconfig::Setting::TypeFloat) = totalRotationX;
    rotation.add("y", libconfig::Setting::TypeFloat) = totalRotationY;
    rotation.add("z", libconfig::Setting::TypeFloat) = totalRotationZ;

    originalRotation.X = totalRotationX;
    originalRotation.Y = totalRotationY;
    originalRotation.Z = totalRotationZ;
    rotatedX = 0.0;
    rotatedY = 0.0;
    rotatedZ = 0.0;

    setting->add("fieldOfView", libconfig::Setting::TypeFloat) = fov;
}

}  // namespace RayTracer
