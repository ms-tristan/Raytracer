// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test for Vector2D class
*/

#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>
#include "Math/Vector2D/Vector2D.hpp"

namespace RayTracerTest {

class Vector2DTest : public ::testing::Test {
 protected:
    void SetUp() override {
        v1 = Math::Vector2D(1.0, 2.0);
        v2 = Math::Vector2D(3.0, 4.0);
        zero = Math::Vector2D(0.0, 0.0);
    }

    Math::Vector2D v1;
    Math::Vector2D v2;
    Math::Vector2D zero;
};

TEST_F(Vector2DTest, DefaultConstructorTest) {
    Math::Vector2D v;
    EXPECT_DOUBLE_EQ(0.0, v.U);
    EXPECT_DOUBLE_EQ(0.0, v.V);
}

TEST_F(Vector2DTest, ParameterizedConstructorTest) {
    Math::Vector2D v(2.5, 3.5);
    EXPECT_DOUBLE_EQ(2.5, v.U);
    EXPECT_DOUBLE_EQ(3.5, v.V);
}

TEST_F(Vector2DTest, Coords2DConstructorTest) {
    Math::Coords2D coords{2.5, 3.5};
    Math::Vector2D v(coords);
    EXPECT_DOUBLE_EQ(2.5, v.U);
    EXPECT_DOUBLE_EQ(3.5, v.V);
}

TEST_F(Vector2DTest, AdditionTest) {
    Math::Vector2D sum = v1 + v2;
    EXPECT_DOUBLE_EQ(4.0, sum.U);
    EXPECT_DOUBLE_EQ(6.0, sum.V);
}

TEST_F(Vector2DTest, SubtractionTest) {
    Math::Vector2D diff = v1 - v2;
    EXPECT_DOUBLE_EQ(-2.0, diff.U);
    EXPECT_DOUBLE_EQ(-2.0, diff.V);
}

TEST_F(Vector2DTest, ScalarMultiplicationTest) {
    Math::Vector2D prod = v1 * 2.0;
    EXPECT_DOUBLE_EQ(2.0, prod.U);
    EXPECT_DOUBLE_EQ(4.0, prod.V);

    Math::Vector2D prod2 = 3.0 * v1;
    EXPECT_DOUBLE_EQ(3.0, prod2.U);
    EXPECT_DOUBLE_EQ(6.0, prod2.V);
}

TEST_F(Vector2DTest, ScalarDivisionTest) {
    Math::Vector2D quotient = v1 / 2.0;
    EXPECT_DOUBLE_EQ(0.5, quotient.U);
    EXPECT_DOUBLE_EQ(1.0, quotient.V);
}

TEST_F(Vector2DTest, ScalarDivisionByZeroTest) {
    EXPECT_THROW(v1 / 0.0, std::runtime_error);
}

TEST_F(Vector2DTest, MagnitudeTest) {
    double expected = std::sqrt(1.0*1.0 + 2.0*2.0);
    EXPECT_DOUBLE_EQ(expected, v1.magnitude());
    EXPECT_DOUBLE_EQ(0.0, zero.magnitude());
}

TEST_F(Vector2DTest, NormalizeTest) {
    Math::Vector2D normalized = v1.normalize();
    double magnitude = v1.magnitude();
    EXPECT_DOUBLE_EQ(v1.U / magnitude, normalized.U);
    EXPECT_DOUBLE_EQ(v1.V / magnitude, normalized.V);
    EXPECT_NEAR(1.0, normalized.magnitude(), 1e-10);

    Math::Vector2D normalizedZero = zero.normalize();
    EXPECT_DOUBLE_EQ(0.0, normalizedZero.U);
    EXPECT_DOUBLE_EQ(0.0, normalizedZero.V);
}

TEST_F(Vector2DTest, CompoundAdditionTest) {
    Math::Vector2D v = v1;
    v += v2;
    EXPECT_DOUBLE_EQ(4.0, v.U);
    EXPECT_DOUBLE_EQ(6.0, v.V);
}

TEST_F(Vector2DTest, CompoundSubtractionTest) {
    Math::Vector2D v = v1;
    v -= v2;
    EXPECT_DOUBLE_EQ(-2.0, v.U);
    EXPECT_DOUBLE_EQ(-2.0, v.V);
}

}  // namespace RayTracerTest