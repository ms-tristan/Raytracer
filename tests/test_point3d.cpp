// filepath: /home/alex/Raytracer/tests/test_point3d.cpp
// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test for Point3D class
*/

#include <gtest/gtest.h>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"

namespace RayTracerTest {

class Point3DTest : public ::testing::Test {
 protected:
    void SetUp() override {
        p1 = Math::Point3D({1.0, 2.0, 3.0});
        p2 = Math::Point3D({4.0, 5.0, 6.0});
        v1 = Math::Vector3D({2.0, 3.0, 4.0});
    }

    Math::Point3D p1;
    Math::Point3D p2;
    Math::Vector3D v1;
};

TEST_F(Point3DTest, ConstructorTest) {
    Math::Point3D p({2.5, 3.5, 4.5});
    EXPECT_DOUBLE_EQ(2.5, p.X);
    EXPECT_DOUBLE_EQ(3.5, p.Y);
    EXPECT_DOUBLE_EQ(4.5, p.Z);
}

TEST_F(Point3DTest, AddVectorTest) {
    Math::Point3D result = p1 + v1;
    EXPECT_DOUBLE_EQ(3.0, result.X);
    EXPECT_DOUBLE_EQ(5.0, result.Y);
    EXPECT_DOUBLE_EQ(7.0, result.Z);
}

TEST_F(Point3DTest, CompoundAddVectorTest) {
    Math::Point3D result = p1;
    result += v1;
    EXPECT_DOUBLE_EQ(3.0, result.X);
    EXPECT_DOUBLE_EQ(5.0, result.Y);
    EXPECT_DOUBLE_EQ(7.0, result.Z);
}

TEST_F(Point3DTest, SubtractVectorTest) {
    Math::Point3D result = p1 - v1;
    EXPECT_DOUBLE_EQ(-1.0, result.X);
    EXPECT_DOUBLE_EQ(-1.0, result.Y);
    EXPECT_DOUBLE_EQ(-1.0, result.Z);
}

TEST_F(Point3DTest, CompoundSubtractVectorTest) {
    Math::Point3D result = p1;
    result -= v1;
    EXPECT_DOUBLE_EQ(-1.0, result.X);
    EXPECT_DOUBLE_EQ(-1.0, result.Y);
    EXPECT_DOUBLE_EQ(-1.0, result.Z);
}

TEST_F(Point3DTest, SubtractPointTest) {
    Math::Vector3D result = p1 - p2;
    EXPECT_DOUBLE_EQ(-3.0, result.X);
    EXPECT_DOUBLE_EQ(-3.0, result.Y);
    EXPECT_DOUBLE_EQ(-3.0, result.Z);
}

TEST_F(Point3DTest, EdgeCasesTest) {
    Math::Vector3D zeroVector({0.0, 0.0, 0.0});
    Math::Point3D result = p1 + zeroVector;
    EXPECT_DOUBLE_EQ(p1.X, result.X);
    EXPECT_DOUBLE_EQ(p1.Y, result.Y);
    EXPECT_DOUBLE_EQ(p1.Z, result.Z);

    Math::Point3D origin({0.0, 0.0, 0.0});
    Math::Vector3D toOrigin = origin - p1;
    EXPECT_DOUBLE_EQ(-p1.X, toOrigin.X);
    EXPECT_DOUBLE_EQ(-p1.Y, toOrigin.Y);
    EXPECT_DOUBLE_EQ(-p1.Z, toOrigin.Z);
}

}  // namespace RayTracerTest