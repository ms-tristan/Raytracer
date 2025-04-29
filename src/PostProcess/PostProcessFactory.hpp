// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PostProcessFactory
*/

#ifndef SRC_POSTPROCESS_POSTPROCESSFACTORY_HPP_
#define SRC_POSTPROCESS_POSTPROCESSFACTORY_HPP_

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "PostProcess/IPostProcess.hpp"

namespace RayTracer {

class PostProcessFactory {
 private:
    using CreatorFunc = std::function<std::shared_ptr<IPostProcess>(
        const std::map<std::string, double>&)>;
    
    static std::map<std::string, CreatorFunc> creators;
    static bool isInitialized;

 public:
    static std::shared_ptr<IPostProcess> createPostProcess(
        const std::string& type,
        const std::map<std::string, double>& params);

    static void registerPostProcess(const std::string& type, CreatorFunc creator);

    static std::vector<std::string> getAvailablePostProcessTypes();
};

}  // namespace RayTracer

#endif  // SRC_POSTPROCESS_POSTPROCESSFACTORY_HPP_