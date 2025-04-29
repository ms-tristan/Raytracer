// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** GrayscalePostProcess
*/

#ifndef SRC_POSTPROCESS_GRAYSCALEPOSTPROCESS_GRAYSCALEPOSTPROCESS_HPP_
    #define SRC_POSTPROCESS_GRAYSCALEPOSTPROCESS_GRAYSCALEPOSTPROCESS_HPP_
    #include <memory>
    #include <vector>
    #include <string>
    #include "PostProcess/IPostProcess.hpp"

namespace RayTracer {

class GrayscalePostProcess : public IPostProcess {
 private:
    double intensity;

 public:
    explicit GrayscalePostProcess(double intensity = 1.0);
    ~GrayscalePostProcess() override = default;

    std::vector<Math::Vector3D> processFrameBuffer(
        const std::vector<Math::Vector3D>& frameBuffer,
        int width, int height) const override;

    std::string getTypeName() const override;

    static std::string getTypeNameStatic();

    std::shared_ptr<IPostProcess> clone() const override;

    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_GRAYSCALEPOSTPROCESS_GRAYSCALEPOSTPROCESS_HPP_
