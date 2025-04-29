// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IPostProcess interface
*/

#ifndef SRC_POSTPROCESS_IPOSTPROCESS_HPP_
    #define SRC_POSTPROCESS_IPOSTPROCESS_HPP_
    #include <memory>
    #include <vector>
    #include <optional>
    #include <string>
    #include <libconfig.h++>
    #include "Math/Vector3D/Vector3D.hpp"

namespace RayTracer {

class IPostProcess {
 public:
    virtual ~IPostProcess() = default;

    virtual std::vector<Math::Vector3D> processFrameBuffer(
        const std::vector<Math::Vector3D>& frameBuffer,
        int width, int height) const = 0;

    virtual std::string getTypeName() const = 0;

    virtual std::shared_ptr<IPostProcess> clone() const = 0;
    virtual void getLibConfigParams(libconfig::Setting& setting) const = 0;
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_IPOSTPROCESS_HPP_
