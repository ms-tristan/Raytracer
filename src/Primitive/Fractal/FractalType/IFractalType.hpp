// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Fractal Type Interface
*/

#ifndef SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_IFRACTALTYPE_HPP_
    #define SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_IFRACTALTYPE_HPP_
    #include <string>
    #include <memory>
    #include "Math/Point3D/Point3D.hpp"

namespace RayTracer {

class IFractalType {
 public:
    virtual ~IFractalType() = default;
    virtual std::string getName() const = 0;
    virtual double distanceEstimator(const Math::Point3D& point,
                                    const Math::Point3D& center,
                                    int maxIterations,
                                    double bailout,
                                    double power) const = 0;
    virtual std::shared_ptr<IFractalType> clone() const = 0;
};

}  // namespace RayTracer

#endif  // SRC_PRIMITIVE_FRACTAL_FRACTALTYPE_IFRACTALTYPE_HPP_
