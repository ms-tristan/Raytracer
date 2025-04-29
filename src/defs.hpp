/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** defs
*/

#ifndef SRC_DEFS_HPP_
    #define SRC_DEFS_HPP_
    #include "Math/Point3D/Point3D.hpp"
    #include "Math/Vector3D/Vector3D.hpp"

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

class IPrimitive;

struct HitInfo {
    double distance;
    Math::Point3D hitPoint;
    Math::Vector3D normal;
    const IPrimitive *primitive;
};

}  // namespace RayTracer

#endif  // SRC_DEFS_HPP_
