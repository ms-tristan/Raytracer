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

void Camera::translate(const Math::Vector3D &translation) { 
    origin += translation; 
    screen.origin += translation; 
}

void Camera::rotateX(double degrees) {
    // Calculate current view direction and screen dimensions
    Math::Point3D screenCenter = screen.origin + 
        screen.bottom_side * 0.5 + screen.left_side * 0.5;
    Math::Vector3D viewDir = (screenCenter - origin).normalize();
    Math::Vector3D rightDir = screen.bottom_side.normalize();
    Math::Vector3D upDir = screen.left_side.normalize();
    double screenDistance = (screenCenter - origin).length();
    double screenWidth = screen.bottom_side.length();
    double screenHeight = screen.left_side.length();
    
    // Rotate view direction around the right vector
    double angleRad = Math::toRadians(degrees);
    double cosAngle = cos(angleRad);
    double sinAngle = sin(angleRad);
    
    // Rodrigues rotation formula for rotation around arbitrary axis (rightDir)
    Math::Vector3D newViewDir = viewDir * cosAngle + 
                          rightDir.cross(viewDir) * sinAngle + 
                          rightDir * rightDir.dot(viewDir) * (1 - cosAngle);
    newViewDir = newViewDir.normalize();
    
    // Calculate new up vector perpendicular to right and new view direction
    Math::Vector3D newUpDir = rightDir.cross(newViewDir).normalize();
    
    // Reconstruct screen
    Math::Point3D newScreenCenter = origin + newViewDir * screenDistance;
    Math::Vector3D halfWidthVec = rightDir * (screenWidth/2);
    Math::Vector3D halfHeightVec = newUpDir * (screenHeight/2);
    
    screen.origin = newScreenCenter - halfWidthVec - halfHeightVec;
    screen.bottom_side = rightDir * screenWidth;
    screen.left_side = newUpDir * screenHeight;
}

void Camera::rotateY(double degrees) {
    // Calculate current view direction and screen dimensions
    Math::Point3D screenCenter = screen.origin + 
        screen.bottom_side * 0.5 + screen.left_side * 0.5;
    Math::Vector3D viewDir = (screenCenter - origin).normalize();
    double screenDistance = (screenCenter - origin).length();
    double screenWidth = screen.bottom_side.length();
    double screenHeight = screen.left_side.length();
    
    // Rotate the view direction around the global Y axis
    Math::Vector3D newViewDir = Math::Rotation::rotateY(viewDir, degrees);
    newViewDir = newViewDir.normalize();
    
    // Calculate right vector perpendicular to newViewDir and global up (0,1,0)
    Math::Vector3D globalUp(Math::Coords{0.0, 1.0, 0.0});
    Math::Vector3D rightDir = newViewDir.cross(globalUp).normalize();
    
    // If view is too close to up/down, use different reference vector
    if (rightDir.length() < 0.001) {
        Math::Vector3D tempRef(Math::Coords{1.0, 0.0, 0.0});
        rightDir = newViewDir.cross(tempRef).normalize();
    }
    
    // Calculate camera's up vector from right and view direction
    Math::Vector3D upDir = rightDir.cross(newViewDir).normalize();
    
    // Reconstruct screen
    Math::Point3D newScreenCenter = origin + newViewDir * screenDistance;
    Math::Vector3D halfWidthVec = rightDir * (screenWidth/2);
    Math::Vector3D halfHeightVec = upDir * (screenHeight/2);
    
    screen.origin = newScreenCenter - halfWidthVec - halfHeightVec;
    screen.bottom_side = rightDir * screenWidth;
    screen.left_side = upDir * screenHeight;
}

void Camera::rotateZ(double degrees) {
    // Calculate current view direction and screen dimensions
    Math::Point3D screenCenter = screen.origin + 
        screen.bottom_side * 0.5 + screen.left_side * 0.5;
    Math::Vector3D viewDir = (screenCenter - origin).normalize();
    double screenDistance = (screenCenter - origin).length();
    double screenWidth = screen.bottom_side.length();
    double screenHeight = screen.left_side.length();
    
    // Keep the view direction the same, but rotate the camera's local coordinate system
    Math::Vector3D rightDir = Math::Rotation::rotateZ(screen.bottom_side.normalize(), degrees);
    Math::Vector3D upDir = Math::Rotation::rotateZ(screen.left_side.normalize(), degrees);
    
    // Ensure orthogonality (view direction remains the same)
    rightDir = rightDir.normalize();
    upDir = upDir.normalize();
    
    // Reconstruct screen
    Math::Point3D newScreenCenter = origin + viewDir * screenDistance;
    Math::Vector3D halfWidthVec = rightDir * (screenWidth/2);
    Math::Vector3D halfHeightVec = upDir * (screenHeight/2);
    
    screen.origin = newScreenCenter - halfWidthVec - halfHeightVec;
    screen.bottom_side = rightDir * screenWidth;
    screen.left_side = upDir * screenHeight;
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
