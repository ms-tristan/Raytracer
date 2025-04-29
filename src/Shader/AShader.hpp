// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShader abstract base class
*/

#ifndef SRC_SHADER_ASHADER_HPP_
#define SRC_SHADER_ASHADER_HPP_

#include <memory>
#include <string>
#include <libconfig.h++>
#include "IShader.hpp"

namespace RayTracer {

class AShader : public IShader {
 protected:
    std::string type;

 public:
    explicit AShader(const std::string& shaderType);
    virtual ~AShader() = default;

    std::string getType() const override;
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_ASHADER_HPP_
