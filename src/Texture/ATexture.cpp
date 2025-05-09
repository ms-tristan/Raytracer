// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ATexture implementation
*/

#include "ATexture.hpp"
#include <string>

namespace RayTracer {

ATexture::ATexture(const std::string& textureType) : type(textureType) {}

std::string ATexture::getType() const {
    return type;
}

}  // namespace RayTracer
