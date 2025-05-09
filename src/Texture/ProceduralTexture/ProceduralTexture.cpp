// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ProceduralTexture implementation
*/

#include "ProceduralTexture.hpp"
#include <memory>
#include <iostream>
#include <vector>
#include <string>

namespace RayTracer {

ProceduralTexture::ProceduralTexture(const std::string& type,
                                     const Math::Vector3D& primaryColor,
                                     const Math::Vector3D& secondaryColor,
                                     double textureScale)
: ATexture(type), color1(primaryColor), color2(secondaryColor), scale(textureScale) {}

const Math::Vector3D& ProceduralTexture::getPrimaryColor() const {
    return color1;
}

const Math::Vector3D& ProceduralTexture::getSecondaryColor() const {
    return color2;
}

double ProceduralTexture::getScale() const {
    return scale;
}

void ProceduralTexture::setPrimaryColor(const Math::Vector3D& color) {
    color1 = color;
}

void ProceduralTexture::setSecondaryColor(const Math::Vector3D& color) {
    color2 = color;
}

void ProceduralTexture::setScale(double newScale) {
    scale = newScale;
}

}  // namespace RayTracer
