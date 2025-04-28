// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** AShader
*/

#include "AShader.hpp"

namespace RayTracer {

AShader::AShader(const std::string& shaderType) : type(shaderType) {}

std::string AShader::getType() const {
    return type;
}

void AShader::getLibConfigParams(libconfig::Setting& setting) const {
    setting.add("type", libconfig::Setting::TypeString) = type;
}

}  // namespace RayTracer