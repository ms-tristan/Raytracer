// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ITransformation interface
*/

#ifndef SRC_TRANSFORMATION_ITRANSFORMATION_HPP_
#define SRC_TRANSFORMATION_ITRANSFORMATION_HPP_
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class ITransformation {
 public:
    virtual ~ITransformation() = default;

    virtual Math::Vector3D applyToVector(const Math::Vector3D& vector) const = 0;
    virtual Math::Point3D applyToPoint(const Math::Point3D& point) const = 0;
};

}  // namespace RayTracer

#endif  // SRC_TRANSFORMATION_ITRANSFORMATION_HPP_
