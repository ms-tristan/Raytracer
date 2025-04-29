// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ChromaticAberrationPostProcess
*/

#ifndef SRC_POSTPROCESS_CHROMATICABERRATIONPOSTPROCESS_CHROMATICABERRATIONPOSTPROCESS_HPP_
#define SRC_POSTPROCESS_CHROMATICABERRATIONPOSTPROCESS_CHROMATICABERRATIONPOSTPROCESS_HPP_

#include <memory>
#include <vector>
#include <mutex>
#include "PostProcess/IPostProcess.hpp"

namespace RayTracer {

class ChromaticAberrationPostProcess : public IPostProcess {
 private:
    double strength;
    mutable std::vector<Math::Vector3D> imageBuffer;
    mutable std::mutex bufferMutex;
    mutable int processedPixels = 0;
    mutable int totalPixels = 0;
    mutable bool bufferFilled = false;

 public:
    explicit ChromaticAberrationPostProcess(double strength = 3.0);
    ~ChromaticAberrationPostProcess() override = default;

    std::vector<Math::Vector3D> processFrameBuffer(
        const std::vector<Math::Vector3D>& frameBuffer,
        int width, int height) const override;

    std::string getTypeName() const override;
    static std::string getTypeNameStatic();

    std::shared_ptr<IPostProcess> clone() const override;
    void getLibConfigParams(libconfig::Setting& setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_CHROMATICABERRATIONPOSTPROCESS_CHROMATICABERRATIONPOSTPROCESS_HPP_