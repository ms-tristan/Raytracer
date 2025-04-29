// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** PointLight implementation
*/

#include "Light/PointLight/PointLight.hpp"
#include <memory>

namespace RayTracer {
PointLight::PointLight(const Math::Point3D &position,
const Math::Vector3D &color, double constant, double linear, double quadratic)
: ALight(color), position(position), constantAttenuation(constant),
linearAttenuation(linear), quadraticAttenuation(quadratic) {}

Math::Vector3D PointLight::getLightDirection(const Math::Point3D &point) const {
    Math::Vector3D direction = position - point;
    return direction.normalize();
}

Math::Vector3D PointLight::getLightColor(const Math::Point3D &point) const {
    Math::Vector3D distanceVector = position - point;
    double distance = distanceVector.length();

    double attenuation = 1.0 / (constantAttenuation +
        linearAttenuation * distance +
        quadraticAttenuation * distance * distance);

    return color * attenuation;
}

std::shared_ptr<ILight> PointLight::clone() const {
    return std::make_shared<PointLight>(
        position, color, constantAttenuation,
        linearAttenuation, quadraticAttenuation);
}

void PointLight::getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const {
    libconfig::Setting& pos = setting->add("position", libconfig::Setting::TypeGroup);
    pos.add("x", libconfig::Setting::TypeFloat) = position.X;
    pos.add("y", libconfig::Setting::TypeFloat) = position.Y;
    pos.add("z", libconfig::Setting::TypeFloat) = position.Z;

    libconfig::Setting& colorSetting = setting->add("color", libconfig::Setting::TypeGroup);
    colorSetting.add("r", libconfig::Setting::TypeFloat) = color.X;
    colorSetting.add("g", libconfig::Setting::TypeFloat) = color.Y;
    colorSetting.add("b", libconfig::Setting::TypeFloat) = color.Z;

    libconfig::Setting& attenuation = setting->add("attenuation", libconfig::Setting::TypeGroup);
    attenuation.add("constant", libconfig::Setting::TypeFloat) = constantAttenuation;
    attenuation.add("linear", libconfig::Setting::TypeFloat) = linearAttenuation;
    attenuation.add("quadratic", libconfig::Setting::TypeFloat) = quadraticAttenuation;
}

}  // namespace RayTracer
