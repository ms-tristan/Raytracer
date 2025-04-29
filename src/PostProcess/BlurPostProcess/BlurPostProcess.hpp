// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BlurPostProcess
*/

#ifndef SRC_POSTPROCESS_BLURPOSTPROCESS_BLURPOSTPROCESS_HPP_
#define SRC_POSTPROCESS_BLURPOSTPROCESS_BLURPOSTPROCESS_HPP_

#include <memory>
#include <vector>
#include <mutex>
#include <atomic>
#include "PostProcess/IPostProcess.hpp"

namespace RayTracer {

class BlurPostProcess : public IPostProcess {
 private:
    double radius;
    mutable std::vector<Math::Vector3D> imageBuffer;
    mutable std::mutex bufferMutex;
    mutable int processedPixels = 0;
    mutable int totalPixels = 0;
    mutable bool bufferFilled = false;
    mutable std::atomic<int> frameCounter{0};
    mutable int lastProcessedFrame = -1;

 public:
    explicit BlurPostProcess(double radius = 3.0);
    ~BlurPostProcess() override = default;

    std::vector<Math::Vector3D> processFrameBuffer(
        const std::vector<Math::Vector3D>& frameBuffer,
        int width, int height) const override;

    std::string getTypeName() const override;

    static std::string getTypeNameStatic();

    std::shared_ptr<IPostProcess> clone() const override;
    void getLibConfigParams(libconfig::Setting& setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_BLURPOSTPROCESS_BLURPOSTPROCESS_HPP_