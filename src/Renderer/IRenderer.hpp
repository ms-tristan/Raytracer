/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Renderer interface
*/

#ifndef RAYTRACER_IRENDERER_HPP_
#define RAYTRACER_IRENDERER_HPP_

#include "../Scene/Scene.hpp"
#include "../Camera/Camera.hpp"

namespace RayTracer {

class IRenderer {
 public:
    virtual ~IRenderer() = default;

    virtual void drawScene(const Scene& scene, const Camera& camera) = 0;

};

} // namespace RayTracer

#endif // RAYTRACER_IRENDERER_HPP_
