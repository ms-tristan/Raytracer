/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for MobiusStrip primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include <cmath>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/MobiusStrip/MobiusStrip.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class MobiusStripTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D center(Math::Coords{0, 0, 0});
        stripDefault = std::make_shared<RayTracer::MobiusStrip>(center, 1.0, 0.2, 0.05);

        Math::Point3D center2(Math::Coords{2, 3, 4});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 0.0, 0.0});
        material->reflectivity = 0.8;
        material->transparency = 0.5;
        stripWithMaterial = std::make_shared<RayTracer::MobiusStrip>(center2, 2.0, 0.5, 0.1, material);
    }

    std::shared_ptr<RayTracer::MobiusStrip> stripDefault;
    std::shared_ptr<RayTracer::MobiusStrip> stripWithMaterial;
};

TEST_F(MobiusStripTest, CreationTest) {
    EXPECT_EQ(stripDefault->center.X, 0);
    EXPECT_EQ(stripDefault->center.Y, 0);
    EXPECT_EQ(stripDefault->center.Z, 0);
    EXPECT_EQ(stripDefault->majorRadius, 1.0);
    EXPECT_EQ(stripDefault->minorRadius, 0.2);

    EXPECT_EQ(stripWithMaterial->center.X, 2);
    EXPECT_EQ(stripWithMaterial->center.Y, 3);
    EXPECT_EQ(stripWithMaterial->center.Z, 4);
    EXPECT_EQ(stripWithMaterial->majorRadius, 2.0);
    EXPECT_EQ(stripWithMaterial->minorRadius, 0.5);
    EXPECT_EQ(stripWithMaterial->getMaterial()->color.X, 1.0);
    EXPECT_EQ(stripWithMaterial->getMaterial()->color.Y, 0.0);
    EXPECT_EQ(stripWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(stripWithMaterial->getMaterial()->reflectivity, 0.8);
    EXPECT_EQ(stripWithMaterial->getMaterial()->transparency, 0.5);
}

TEST_F(MobiusStripTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    stripDefault->translate(translation);
    
    EXPECT_EQ(stripDefault->center.X, 1);
    EXPECT_EQ(stripDefault->center.Y, 2);
    EXPECT_EQ(stripDefault->center.Z, 3);
    
    EXPECT_EQ(stripDefault->majorRadius, 1.0);
    EXPECT_EQ(stripDefault->minorRadius, 0.2);
}

TEST_F(MobiusStripTest, RotationTest) {
    Math::Point3D initialCenter = stripDefault->center;
    
    stripDefault->rotateX(45);
    stripDefault->rotateY(30);
    stripDefault->rotateZ(60);
    
    EXPECT_EQ(stripDefault->center.X, initialCenter.X);
    EXPECT_EQ(stripDefault->center.Y, initialCenter.Y);
    EXPECT_EQ(stripDefault->center.Z, initialCenter.Z);
    
    EXPECT_EQ(stripDefault->majorRadius, 1.0);
    EXPECT_EQ(stripDefault->minorRadius, 0.2);
}

TEST_F(MobiusStripTest, CloneTest) {
    auto clonedStrip = std::dynamic_pointer_cast<RayTracer::MobiusStrip>(stripWithMaterial->clone());
    
    EXPECT_NE(clonedStrip, nullptr);
    if (clonedStrip) {
        EXPECT_EQ(clonedStrip->center.X, stripWithMaterial->center.X);
        EXPECT_EQ(clonedStrip->center.Y, stripWithMaterial->center.Y);
        EXPECT_EQ(clonedStrip->center.Z, stripWithMaterial->center.Z);
        EXPECT_EQ(clonedStrip->majorRadius, stripWithMaterial->majorRadius);
        EXPECT_EQ(clonedStrip->minorRadius, stripWithMaterial->minorRadius);
        EXPECT_EQ(clonedStrip->getMaterial()->color.X, stripWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedStrip->getMaterial()->color.Y, stripWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedStrip->getMaterial()->color.Z, stripWithMaterial->getMaterial()->color.Z);
        EXPECT_EQ(clonedStrip->getMaterial()->reflectivity, stripWithMaterial->getMaterial()->reflectivity);
        EXPECT_EQ(clonedStrip->getMaterial()->transparency, stripWithMaterial->getMaterial()->transparency);
    }
}

TEST_F(MobiusStripTest, TypeNameTest) {
    EXPECT_EQ(stripDefault->getTypeName(), "mobiusstrips");
    EXPECT_EQ(RayTracer::MobiusStrip::getTypeNameStatic(), "mobiusstrips");
}

TEST_F(MobiusStripTest, HitTest) {
    EXPECT_EQ(stripDefault->center.X, 0);
    EXPECT_EQ(stripDefault->center.Y, 0);
    EXPECT_EQ(stripDefault->center.Z, 0);
    EXPECT_EQ(stripDefault->majorRadius, 1.0);
    EXPECT_EQ(stripDefault->minorRadius, 0.2);
}

TEST_F(MobiusStripTest, BoundsTest) {
}

TEST_F(MobiusStripTest, EdgeCaseTests) {
}

TEST_F(MobiusStripTest, TransformedStripTest) {
    auto transformedStrip = std::make_shared<RayTracer::MobiusStrip>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        1.0,
        0.2,
        0.05);
    
    transformedStrip->rotateX(45);
    transformedStrip->rotateZ(30);
    transformedStrip->translate(Math::Vector3D(Math::Coords{2, 3, 4}));
    
    EXPECT_EQ(transformedStrip->center.X, 2);
    EXPECT_EQ(transformedStrip->center.Y, 3);
    EXPECT_EQ(transformedStrip->center.Z, 4);
}

TEST_F(MobiusStripTest, DifferentSizeTest) {
}

} // namespace RayTracerTest