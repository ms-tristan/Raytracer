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
    Renderer(std::shared_ptr<IDisplayManager> displayManager);
    ~Renderer();

    void drawScene(const Scene& scene, const Camera& camera) override;

  private:
    std::shared_ptr<IDisplayManager> _displayManager;
};

}  // namespace RayTracer
