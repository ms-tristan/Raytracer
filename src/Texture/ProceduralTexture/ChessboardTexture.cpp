// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ChessboardTexture implementation
*/

#include <cmath>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "ChessboardTexture.hpp"

namespace RayTracer {

ChessboardTexture::ChessboardTexture(const Math::Vector3D& color1,
                                     const Math::Vector3D& color2,
                                     double scale)
: ProceduralTexture("chessboard", color1, color2, scale) {}

Math::Vector3D ChessboardTexture::getColorAt(const Math::Vector2D& uv) const {
    double u = uv.U * scale;
    double v = uv.V * scale;
    if (scale <= 0.0)
        return color1;
    int x = static_cast<int>(std::floor(u));
    int y = static_cast<int>(std::floor(v));
    if ((x + y) % 2 == 0)
        return color1;
    else
        return color2;
}

std::shared_ptr<ITexture> ChessboardTexture::clone() const {
    return std::make_shared<ChessboardTexture>(color1, color2, scale);
}

}  // namespace RayTracer
