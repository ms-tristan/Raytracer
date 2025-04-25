/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** defs
*/

#ifndef RAYTRACER_DEFS_HPP_
#define RAYTRACER_DEFS_HPP_

namespace RayTracer {

struct Color {
  unsigned char r, g, b, a;
};
using color_t = Color;

struct Vector2f {
  float x, y;
};
using vector2f_t = Vector2f;

struct Vector2u {
  unsigned int x, y;
};
using vector2u_t = Vector2u;

struct Vector2i {
  int x, y;
};
using vector2i_t = Vector2i;

struct Recti {
  int x, y, width, height;
};
using recti_t = Recti;

} // namespace RayTracer

#endif // RAYTRACER_DEFS_HPP_
