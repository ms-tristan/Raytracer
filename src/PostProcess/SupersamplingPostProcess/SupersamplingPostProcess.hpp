// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SupersamplingPostProcess
*/

#ifndef SRC_POSTPROCESS_SUPERSAMPLINGPOSTPROCESS_SUPERSAMPLINGPOSTPROCESS_HPP_
#define SRC_POSTPROCESS_SUPERSAMPLINGPOSTPROCESS_SUPERSAMPLINGPOSTPROCESS_HPP_

#include <memory>
#include <vector>
#include <mutex>
#include <string>
#include <atomic>
#include "PostProcess/IPostProcess.hpp"

namespace RayTracer {

/**
 * @brief Post-processor that implements supersampling antialiasing
 *
 * This post-processor doesn't directly modify the framebuffer like other
 * post-processors. Instead, it works at the ray generation stage by generating
 * multiple rays per pixel and averaging the results.
 */
class SupersamplingPostProcess : public IPostProcess {
 private:
    int samplesPerPixel; // Number of samples per pixel (e.g., 4, 9, 16)
    mutable std::vector<Math::Vector3D> imageBuffer;
    mutable std::mutex bufferMutex;

 public:
    explicit SupersamplingPostProcess(int samplesPerPixel = 4);
    ~SupersamplingPostProcess() override = default;

    /**
     * @brief Process the frame buffer by applying supersampling antialiasing
     *
     * @param frameBuffer The original frame buffer
     * @param width The width of the frame buffer
     * @param height The height of the frame buffer
     * @return The processed frame buffer
     */
    std::vector<Math::Vector3D> processFrameBuffer(
        const std::vector<Math::Vector3D>& frameBuffer,
        int width, int height) const override;

    /**
     * @brief Get the number of samples per pixel used by the supersampling algorithm
     *
     * @return The number of samples per pixel
     */
    int getSamplesPerPixel() const { return samplesPerPixel; }

    /**
     * @brief Get a parameter value by name
     *
     * @param paramName The name of the parameter to retrieve
     * @return The parameter value
     */
    double getParameter(const std::string& paramName) const override {
        if (paramName == "samples") {
            return static_cast<double>(samplesPerPixel);
        }
        return 0.0;
    }

    /**
     * @brief Get the type name of this post-process effect
     *
     * @return The type name
     */
    std::string getTypeName() const override;

    /**
     * @brief Get the static type name of this post-process effect
     *
     * @return The static type name
     */
    static std::string getTypeNameStatic();

    /**
     * @brief Clone this post-process effect
     *
     * @return A clone of this post-process effect
     */
    std::shared_ptr<IPostProcess> clone() const override;

    /**
     * @brief Set the parameters for this post-process effect in the scene configuration
     *
     * @param setting The setting to add the parameters to
     */
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_SUPERSAMPLINGPOSTPROCESS_SUPERSAMPLINGPOSTPROCESS_HPP_
