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
    Renderer(IDisplayManager& displayManager);
    ~Renderer();

    void drawScene(const Scene& scene, const Camera& camera) override;

  private:
    IDisplayManager& _displayManager;
};

}  // namespace RayTracer
