/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Renderer interface
*/

#ifndef SRC_RENDERER_IRENDERER_HPP_
    #define SRC_RENDERER_IRENDERER_HPP_
    #include "../Scene/Scene.hpp"
    #include "../Camera/Camera.hpp"

namespace RayTracer {

class IRenderer {
 public:
    virtual ~IRenderer() = default;

    virtual void drawScene( const Scene& scene,
                            const Camera& camera,
                            const bool lowRender = false ) = 0;
};

}  // namespace RayTracer

#endif  // SRC_RENDERER_IRENDERER_HPP_
