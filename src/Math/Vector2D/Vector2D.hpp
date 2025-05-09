// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Vector2D implementation
*/

#ifndef SRC_MATH_VECTOR2D_VECTOR2D_HPP_
    #define SRC_MATH_VECTOR2D_VECTOR2D_HPP_
    #include <iostream>
    #include <cmath>

namespace Math {

struct Coords2D {
    double U;
    double V;
};

class Vector2D {
 public:
    double U;
    double V;

    Vector2D() : U(0.0), V(0.0) {}
    explicit Vector2D(const Coords2D &coords) : U(coords.U), V(coords.V) {}
    Vector2D(double u, double v) : U(u), V(v) {}

    Vector2D operator+(const Vector2D &other) const {
        return Vector2D(U + other.U, V + other.V);
    }

    Vector2D operator-(const Vector2D &other) const {
        return Vector2D(U - other.U, V - other.V);
    }

    Vector2D operator*(double scalar) const {
        return Vector2D(U * scalar, V * scalar);
    }

    Vector2D operator/(double scalar) const {
        if (scalar == 0.0) {
            throw std::runtime_error("Division by zero in Vector2D");
        }
        return Vector2D(U / scalar, V / scalar);
    }

    double magnitude() const {
        return std::sqrt(U * U + V * V);
    }

    Vector2D normalize() const {
        double mag = magnitude();
        if (mag == 0.0) {
            return *this;
        }
        return *this / mag;
    }
};

inline Vector2D operator*(double scalar, const Vector2D &vec) {
    return Vector2D(scalar * vec.U, scalar * vec.V);
}

inline Vector2D &operator+=(Vector2D &lhs, const Vector2D &rhs) {
    lhs.U += rhs.U;
    lhs.V += rhs.V;
    return lhs;
}

inline Vector2D &operator-=(Vector2D &lhs, const Vector2D &rhs) {
    lhs.U -= rhs.U;
    lhs.V -= rhs.V;
    return lhs;
}

}  // namespace Math

#endif  // SRC_MATH_VECTOR2D_VECTOR2D_HPP_
