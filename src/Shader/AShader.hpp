// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShader abstract base class
*/

#ifndef SRC_SHADER_ASHADER_HPP_
#define SRC_SHADER_ASHADER_HPP_

#include "IShader.hpp"
#include <memory>
#include <string>
#include <libconfig.h++>

namespace RayTracer {

class AShader : public IShader {
 protected:
    std::string type;

 public:
    AShader(const std::string& shaderType);
    virtual ~AShader() = default;

    std::string getType() const override;
    virtual void getLibConfigParams(libconfig::Setting& setting) const override;
};

}  // namespace RayTracer

#endif  // SRC_SHADER_ASHADER_HPP_