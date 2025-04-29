// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scale transformation
*/

#ifndef SRC_TRANSFORMATION_SCALE_SCALE_HPP_
#define SRC_TRANSFORMATION_SCALE_SCALE_HPP_
#include "Transformation/ITransformation.hpp"

namespace RayTracer {

class Scale : public ITransformation {
 private:
    double factor;

 public:
    explicit Scale(double factor);
    ~Scale() override = default;

    Math::Vector3D applyToVector(const Math::Vector3D& vector) const override;
    Math::Point3D applyToPoint(const Math::Point3D& point) const override;
};

}  // namespace RayTracer

#endif  // SRC_TRANSFORMATION_SCALE_SCALE_HPP_
