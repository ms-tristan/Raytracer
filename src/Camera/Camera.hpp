// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Camera
*/

#ifndef SRC_CAMERA_CAMERA_HPP_
#define SRC_CAMERA_CAMERA_HPP_

#include <memory>
#include <cmath>
#include <random>
#include <libconfig.h++>

#include "Math/Point3D/Point3D.hpp"
#include "Ray/Ray.hpp"
#include "Rectangle3D/Rectangle3D.hpp"

namespace RayTracer {
// Forward declarations
class Scene;
class SupersamplingPostProcess;

class Camera {
 public:
    Math::Point3D origin;
    Rectangle3D screen;
    double fov;
    mutable Math::Vector3D originalRotation;
    mutable double rotatedX;
    mutable double rotatedY;
    mutable double rotatedZ;

    Camera();
    Camera(const Math::Point3D &origin,
    const Rectangle3D &screen, double fov = 90.0);
    void setFOV(double newFov);
    double getFOV() const;
    Ray ray(double u, double v) const;

    /**
     * @brief Generate multiple rays for supersampling and return the average color
     *
     * @param u Base U coordinate (0.0 - 1.0)
     * @param v Base V coordinate (0.0 - 1.0)
     * @param scene The scene to render
     * @param samplesPerPixel Number of samples to take per pixel
     * @return Average color from all sample rays
     */
    Math::Vector3D supersampleRay(double u, double v, const Scene& scene, int samplesPerPixel) const;

    void rotateX(double degrees);
    void rotateY(double degrees);
    void rotateZ(double degrees);

    void translate(const Math::Vector3D &translation);
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const;
    void setOriginalRotation(const Math::Vector3D& rotation) { originalRotation = rotation; }

 private:
    void updateScreenForFOV();
    Math::Vector3D calculateRotationAngles() const;
};
}  // namespace RayTracer

#endif  // SRC_CAMERA_CAMERA_HPP_
