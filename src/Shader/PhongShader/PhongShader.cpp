// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PhongShader
*/
#include <memory>
#include <cmath>
#include <algorithm>
#include "PhongShader.hpp"

namespace RayTracer {

PhongShader::PhongShader(double specularStrength, double shininess)
    : AShader("phong"), specularStrength(specularStrength), shininess(shininess) {}

Math::Vector3D PhongShader::apply(const Math::Vector3D& color, const HitInfo& hitInfo, const Ray& ray) const {
    Math::Vector3D viewDir = (ray.origin - hitInfo.hitPoint).normalize();
    Math::Vector3D reflectDir = hitInfo.normal * 2.0 * hitInfo.normal.dot(viewDir) - viewDir;

    double spec = std::pow(std::max(0.0, reflectDir.dot(viewDir)), shininess);
    Math::Vector3D specular = Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}) * specularStrength * spec;

    Math::Vector3D result = color + specular;

    result.X = std::min(1.0, std::max(0.0, result.X));
    result.Y = std::min(1.0, std::max(0.0, result.Y));
    result.Z = std::min(1.0, std::max(0.0, result.Z));

    return result;
}

std::shared_ptr<IShader> PhongShader::clone() const {
    return std::make_shared<PhongShader>(specularStrength, shininess);
}

void PhongShader::getLibConfigParams(libconfig::Setting& setting) const {
    AShader::getLibConfigParams(setting);
    setting.add("specularStrength", libconfig::Setting::TypeFloat) = specularStrength;
    setting.add("shininess", libconfig::Setting::TypeFloat) = shininess;
}

}  // namespace RayTracer
