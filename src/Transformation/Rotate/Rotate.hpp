// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Rotate transformation
*/

#ifndef SRC_TRANSFORMATION_ROTATE_ROTATE_HPP_
    #define SRC_TRANSFORMATION_ROTATE_ROTATE_HPP_
    #include <string>
    #include "Transformation/ITransformation.hpp"

namespace RayTracer {

inline double toRadians(double degrees) { return degrees * M_PI / 180.0; }

class Rotate : public ITransformation {
 private:
    std::string axis;
    double angle;

 public:
    Rotate(const std::string& axis, double angle);
    ~Rotate() override = default;

    Math::Vector3D applyToVector(const Math::Vector3D& vector) const override;
    Math::Point3D applyToPoint(const Math::Point3D& point) const override;
};

}  // namespace RayTracer

#endif  // SRC_TRANSFORMATION_ROTATE_ROTATE_HPP_
