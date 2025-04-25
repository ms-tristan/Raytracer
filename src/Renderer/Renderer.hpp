/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Renderer
*/

#pragma once

#include "IRenderer.hpp"
#include "../DisplayManager/IDisplayManager.hpp"

namespace RayTracer {

class Renderer : public IRenderer {
  public:
    Renderer(IDisplayManager& displayManager)
        : _displayManager(displayManager) {};
    ~Renderer();
};

}  // namespace RayTracer
