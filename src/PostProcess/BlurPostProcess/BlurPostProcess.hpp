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
#include <string>
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

    /**
     * @brief Get a parameter value by name
     *
     * @param paramName The name of the parameter to retrieve
     * @return The parameter value
     */
    double getParameter(const std::string& paramName) const override {
        if (paramName == "radius") {
            return radius;
        }
        return 0.0;
    }

    std::string getTypeName() const override;

    static std::string getTypeNameStatic();

    std::shared_ptr<IPostProcess> clone() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_BLURPOSTPROCESS_BLURPOSTPROCESS_HPP_
