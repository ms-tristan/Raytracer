// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** NegativePostProcess
*/

#ifndef SRC_POSTPROCESS_NEGATIVEPOSTPROCESS_NEGATIVEPOSTPROCESS_HPP_
    #define SRC_POSTPROCESS_NEGATIVEPOSTPROCESS_NEGATIVEPOSTPROCESS_HPP_
    #include <memory>
    #include <vector>
    #include <string>
    #include "PostProcess/IPostProcess.hpp"

namespace RayTracer {

class NegativePostProcess : public IPostProcess {
 private:
    double intensity;

 public:
    explicit NegativePostProcess(double intensity = 1.0);
    ~NegativePostProcess() override = default;

    std::vector<Math::Vector3D> processFrameBuffer(
        const std::vector<Math::Vector3D>& frameBuffer,
        int width, int height) const override;

    std::string getTypeName() const override;

    static std::string getTypeNameStatic();

    std::shared_ptr<IPostProcess> clone() const override;

    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_NEGATIVEPOSTPROCESS_NEGATIVEPOSTPROCESS_HPP_
