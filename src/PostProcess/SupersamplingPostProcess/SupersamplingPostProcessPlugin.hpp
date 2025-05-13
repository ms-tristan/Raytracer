// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SupersamplingPostProcessPlugin
*/

#ifndef SRC_POSTPROCESS_SUPERSAMPLINGPOSTPROCESS_SUPERSAMPLINGPOSTPROCESSPLUGIN_HPP_
#define SRC_POSTPROCESS_SUPERSAMPLINGPOSTPROCESS_SUPERSAMPLINGPOSTPROCESSPLUGIN_HPP_

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include "PostProcess/Plugin/IPostProcessPlugin.hpp"
#include "SupersamplingPostProcess.hpp"

namespace RayTracer {

class SupersamplingPostProcessPlugin : public IPostProcessPlugin {
 public:
    ~SupersamplingPostProcessPlugin() override = default;

    std::string getTypeName() const override;

    std::shared_ptr<IPostProcess> createPostProcess(
        const std::map<std::string, double>& params) override;

    std::vector<std::string> getRequiredParameters() const override;

    std::shared_ptr<IPostProcess> createPostProcessFromSetting(
        const libconfig::Setting& setting) override;
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_SUPERSAMPLINGPOSTPROCESS_SUPERSAMPLINGPOSTPROCESSPLUGIN_HPP_
