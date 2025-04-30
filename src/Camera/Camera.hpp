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
#include <libconfig.h++>
#include <cmath>

#include "Math/Point3D/Point3D.hpp"
#include "Ray/Ray.hpp"
#include "Rectangle3D/Rectangle3D.hpp"

namespace RayTracer {
class Camera {
 public:
   Math::Point3D origin;
   Rectangle3D screen;
   double fov;

   Camera();
   Camera(const Math::Point3D &origin,
   const Rectangle3D &screen, double fov = 90.0);
   void setFOV(double newFov);
   double getFOV() const;
   Ray ray(double u, double v) const;
   void rotateX(double degrees);
   void rotateY(double degrees);
   void rotateZ(double degrees);
   void translate(const Math::Vector3D &translation);
   void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const;
 private:
   void updateScreenForFOV();
   Math::Vector3D calculateRotationAngles() const;
};
}  // namespace RayTracer

#endif  // SRC_CAMERA_CAMERA_HPP_
