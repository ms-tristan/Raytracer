// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Translate transformation
*/

#ifndef SRC_TRANSFORMATION_TRANSLATE_TRANSLATE_HPP_
#define SRC_TRANSFORMATION_TRANSLATE_TRANSLATE_HPP_
#include "Transformation/ITransformation.hpp"

namespace RayTracer {

class Translate : public ITransformation {
 private:
    Math::Vector3D vector;

 public:
    explicit Translate(const Math::Vector3D& vector);
    ~Translate() override = default;

    Math::Vector3D applyToVector(const Math::Vector3D& vector) const override;
    Math::Point3D applyToPoint(const Math::Point3D& point) const override;
};

}  // namespace RayTracer

#endif  // SRC_TRANSFORMATION_TRANSLATE_TRANSLATE_HPP_
