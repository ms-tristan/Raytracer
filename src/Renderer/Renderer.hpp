/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Renderer
*/

#pragma once

#include <memory>

#include "IRenderer.hpp"
#include "../DisplayManager/IDisplayManager.hpp"

namespace RayTracer {

class Renderer : public IRenderer {
  public:
    explicit Renderer(std::shared_ptr<IDisplayManager> displayManager);
    ~Renderer();

    void drawScene( const Scene& scene,
                    const Camera& camera,
                    const bool lowRender = false ) override;

  private:
    std::shared_ptr<IDisplayManager> _displayManager;
};

}  // namespace RayTracer
