// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Translate transformation implementation
*/

#include "Transformation/Translate/Translate.hpp"

namespace RayTracer {

Translate::Translate(const Math::Vector3D& vector) : vector(vector) {}

Math::Vector3D Translate::applyToVector(const
Math::Vector3D& inputVector) const {
    return inputVector;
}

Math::Point3D Translate::applyToPoint(const Math::Point3D& point) const {
    return point + vector;
}

}  // namespace RayTracer
