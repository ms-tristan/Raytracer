/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test for Vector3D class
*/

#include <gtest/gtest.h>
#include <cmath>
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracerTest {

class Vector3DTest : public ::testing::Test {
 protected:
    void SetUp() override {
        v1 = Math::Vector3D({1.0, 2.0, 3.0});
        v2 = Math::Vector3D({4.0, 5.0, 6.0});
        zero = Math::Vector3D({0.0, 0.0, 0.0});
    }

    Math::Vector3D v1;
    Math::Vector3D v2;
    Math::Vector3D zero;
};

TEST_F(Vector3DTest, ConstructorTest) {
    Math::Vector3D v({2.5, 3.5, 4.5});
    EXPECT_DOUBLE_EQ(2.5, v.X);
    EXPECT_DOUBLE_EQ(3.5, v.Y);
    EXPECT_DOUBLE_EQ(4.5, v.Z);
}

TEST_F(Vector3DTest, LengthTest) {
    double expected = std::sqrt(1.0*1.0 + 2.0*2.0 + 3.0*3.0);
    EXPECT_DOUBLE_EQ(expected, v1.length());
    EXPECT_DOUBLE_EQ(0.0, zero.length());
}

TEST_F(Vector3DTest, NormalizeTest) {
    Math::Vector3D normalized = v1.normalize();
    double length = v1.length();
    EXPECT_DOUBLE_EQ(v1.X / length, normalized.X);
    EXPECT_DOUBLE_EQ(v1.Y / length, normalized.Y);
    EXPECT_DOUBLE_EQ(v1.Z / length, normalized.Z);
    EXPECT_DOUBLE_EQ(1.0, normalized.length());

    Math::Vector3D normalizedZero = zero.normalize();
    EXPECT_DOUBLE_EQ(0.0, normalizedZero.X);
    EXPECT_DOUBLE_EQ(0.0, normalizedZero.Y);
    EXPECT_DOUBLE_EQ(0.0, normalizedZero.Z);
}

TEST_F(Vector3DTest, CrossProductTest) {
    Math::Vector3D cross = v1.cross(v2);
    EXPECT_DOUBLE_EQ(2.0*6.0 - 3.0*5.0, cross.X);
    EXPECT_DOUBLE_EQ(3.0*4.0 - 1.0*6.0, cross.Y);
    EXPECT_DOUBLE_EQ(1.0*5.0 - 2.0*4.0, cross.Z);
}

TEST_F(Vector3DTest, AdditionTest) {
    Math::Vector3D sum = v1 + v2;
    EXPECT_DOUBLE_EQ(5.0, sum.X);
    EXPECT_DOUBLE_EQ(7.0, sum.Y);
    EXPECT_DOUBLE_EQ(9.0, sum.Z);
}

TEST_F(Vector3DTest, SubtractionTest) {
    Math::Vector3D diff = v1 - v2;
    EXPECT_DOUBLE_EQ(-3.0, diff.X);
    EXPECT_DOUBLE_EQ(-3.0, diff.Y);
    EXPECT_DOUBLE_EQ(-3.0, diff.Z);
}

TEST_F(Vector3DTest, ElementWiseMultiplicationTest) {
    Math::Vector3D prod = v1 * v2;
    EXPECT_DOUBLE_EQ(4.0, prod.X);
    EXPECT_DOUBLE_EQ(10.0, prod.Y);
    EXPECT_DOUBLE_EQ(18.0, prod.Z);
}

TEST_F(Vector3DTest, ElementWiseDivisionTest) {
    Math::Vector3D quotient = v1 / v2;
    EXPECT_DOUBLE_EQ(1.0/4.0, quotient.X);
    EXPECT_DOUBLE_EQ(2.0/5.0, quotient.Y);
    EXPECT_DOUBLE_EQ(3.0/6.0, quotient.Z);
}

TEST_F(Vector3DTest, ScalarMultiplicationTest) {
    Math::Vector3D prod = v1 * 2.0;
    EXPECT_DOUBLE_EQ(2.0, prod.X);
    EXPECT_DOUBLE_EQ(4.0, prod.Y);
    EXPECT_DOUBLE_EQ(6.0, prod.Z);

    Math::Vector3D prod2 = 3.0 * v1;
    EXPECT_DOUBLE_EQ(3.0, prod2.X);
    EXPECT_DOUBLE_EQ(6.0, prod2.Y);
    EXPECT_DOUBLE_EQ(9.0, prod2.Z);
}

TEST_F(Vector3DTest, ScalarDivisionTest) {
    Math::Vector3D quotient = v1 / 2.0;
    EXPECT_DOUBLE_EQ(0.5, quotient.X);
    EXPECT_DOUBLE_EQ(1.0, quotient.Y);
    EXPECT_DOUBLE_EQ(1.5, quotient.Z);
}

TEST_F(Vector3DTest, DotProductTest) {
    double dot = v1.dot(v2);
    EXPECT_DOUBLE_EQ(1.0*4.0 + 2.0*5.0 + 3.0*6.0, dot);
}

TEST_F(Vector3DTest, CompoundAdditionTest) {
    Math::Vector3D v = v1;
    v += v2;
    EXPECT_DOUBLE_EQ(5.0, v.X);
    EXPECT_DOUBLE_EQ(7.0, v.Y);
    EXPECT_DOUBLE_EQ(9.0, v.Z);
}

TEST_F(Vector3DTest, CompoundSubtractionTest) {
    Math::Vector3D v = v1;
    v -= v2;
    EXPECT_DOUBLE_EQ(-3.0, v.X);
    EXPECT_DOUBLE_EQ(-3.0, v.Y);
    EXPECT_DOUBLE_EQ(-3.0, v.Z);
}

TEST_F(Vector3DTest, CompoundElementWiseMultiplicationTest) {
    Math::Vector3D v = v1;
    v *= v2;
    EXPECT_DOUBLE_EQ(4.0, v.X);
    EXPECT_DOUBLE_EQ(10.0, v.Y);
    EXPECT_DOUBLE_EQ(18.0, v.Z);
}

TEST_F(Vector3DTest, CompoundElementWiseDivisionTest) {
    Math::Vector3D v = v1;
    v /= v2;
    EXPECT_DOUBLE_EQ(1.0/4.0, v.X);
    EXPECT_DOUBLE_EQ(2.0/5.0, v.Y);
    EXPECT_DOUBLE_EQ(3.0/6.0, v.Z);
}

TEST_F(Vector3DTest, CompoundScalarMultiplicationTest) {
    Math::Vector3D v = v1;
    v *= 2.0;
    EXPECT_DOUBLE_EQ(2.0, v.X);
    EXPECT_DOUBLE_EQ(4.0, v.Y);
    EXPECT_DOUBLE_EQ(6.0, v.Z);
}

TEST_F(Vector3DTest, CompoundScalarDivisionTest) {
    Math::Vector3D v = v1;
    v /= 2.0;
    EXPECT_DOUBLE_EQ(0.5, v.X);
    EXPECT_DOUBLE_EQ(1.0, v.Y);
    EXPECT_DOUBLE_EQ(1.5, v.Z);
}

}  // namespace RayTracerTest