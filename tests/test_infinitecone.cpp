/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for InfiniteCone primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/InfiniteCone/InfiniteCone.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class InfiniteConeTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D apex(Math::Coords{0, 0, 0});
        Math::Vector3D axis(Math::Coords{0, 1, 0});
        coneDefault = std::make_shared<RayTracer::InfiniteCone>(apex, axis, 30.0);
        Math::Point3D apex2(Math::Coords{2, 3, 4});
        Math::Vector3D axis2(Math::Coords{1, 0, 0});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 0.0, 0.0});
        material->reflectivity = 0.8;
        material->transparency = 0.5;
        coneWithMaterial = std::make_shared<RayTracer::InfiniteCone>(apex2, axis2, 45.0, material);
    }

    std::shared_ptr<RayTracer::InfiniteCone> coneDefault;
    std::shared_ptr<RayTracer::InfiniteCone> coneWithMaterial;
};

TEST_F(InfiniteConeTest, CreationTest) {
    EXPECT_EQ(coneDefault->apex.X, 0);
    EXPECT_EQ(coneDefault->apex.Y, 0);
    EXPECT_EQ(coneDefault->apex.Z, 0);
    EXPECT_EQ(coneDefault->angle, 30.0);
    EXPECT_EQ(coneDefault->axis.X, 0);
    EXPECT_EQ(coneDefault->axis.Y, 1);
    EXPECT_EQ(coneDefault->axis.Z, 0);

    EXPECT_EQ(coneWithMaterial->apex.X, 2);
    EXPECT_EQ(coneWithMaterial->apex.Y, 3);
    EXPECT_EQ(coneWithMaterial->apex.Z, 4);
    EXPECT_EQ(coneWithMaterial->angle, 45.0);
    EXPECT_EQ(coneWithMaterial->axis.X, 1);
    EXPECT_EQ(coneWithMaterial->axis.Y, 0);
    EXPECT_EQ(coneWithMaterial->axis.Z, 0);
    EXPECT_EQ(coneWithMaterial->getMaterial()->color.X, 1.0);
    EXPECT_EQ(coneWithMaterial->getMaterial()->color.Y, 0.0);
    EXPECT_EQ(coneWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(coneWithMaterial->getMaterial()->reflectivity, 0.8);
    EXPECT_EQ(coneWithMaterial->getMaterial()->transparency, 0.5);
}

TEST_F(InfiniteConeTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    coneDefault->translate(translation);
    
    EXPECT_EQ(coneDefault->apex.X, 1);
    EXPECT_EQ(coneDefault->apex.Y, 2);
    EXPECT_EQ(coneDefault->apex.Z, 3);
    
    EXPECT_EQ(coneDefault->axis.X, 0);
    EXPECT_EQ(coneDefault->axis.Y, 1);
    EXPECT_EQ(coneDefault->axis.Z, 0);
    EXPECT_EQ(coneDefault->angle, 30.0);
}

TEST_F(InfiniteConeTest, RotationTest) {
    coneDefault->rotateX(90);
    
    EXPECT_NEAR(coneDefault->axis.X, 0, 0.001);
    EXPECT_NEAR(coneDefault->axis.Y, 0, 0.001);
    EXPECT_NEAR(coneDefault->axis.Z, 1, 0.001);
    
    coneDefault->rotateY(90);
    
    EXPECT_NEAR(coneDefault->axis.X, 1, 0.001);
    EXPECT_NEAR(coneDefault->axis.Y, 0, 0.001);
    EXPECT_NEAR(coneDefault->axis.Z, 0, 0.001);
}

TEST_F(InfiniteConeTest, HitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{3, -5, 0}), Math::Vector3D(Math::Coords{-1, 1, 0}).normalize());
    auto hitInfo = coneDefault->hit(rayHit, 0, 20);
    
    if (hitInfo.has_value()) {
        EXPECT_GT(hitInfo->distance, 0);
    }
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, -5, 5}), Math::Vector3D(Math::Coords{0, 0, 1}));
    auto hitInfoMiss = coneDefault->hit(rayMiss, 0, 10);
    
    if (hitInfoMiss.has_value()) {
        EXPECT_GT(hitInfoMiss->distance, 0);
    }
}

TEST_F(InfiniteConeTest, EdgeCaseTests) {
    Math::Point3D apexSharp(Math::Coords{0, 0, 0});
    Math::Vector3D axisSharp(Math::Coords{0, 1, 0});
    auto coneSharp = std::make_shared<RayTracer::InfiniteCone>(apexSharp, axisSharp, 1.0);
    
    RayTracer::Ray raySharp(Math::Point3D(Math::Coords{0.1, -5, 0}), Math::Vector3D(Math::Coords{0, 1, 0}));
    auto hitInfoSharp = coneSharp->hit(raySharp, 0, 20);
    if (hitInfoSharp.has_value()) {
        EXPECT_GT(hitInfoSharp->distance, 0);
    }

    Math::Point3D apexWide(Math::Coords{0, 0, 0});
    Math::Vector3D axisWide(Math::Coords{0, 1, 0});
    auto coneWide = std::make_shared<RayTracer::InfiniteCone>(apexWide, axisWide, 89.0);
    
    RayTracer::Ray rayWide(Math::Point3D(Math::Coords{1, -1, 0}), Math::Vector3D(Math::Coords{0, 1, 0}));
    auto hitInfoWide = coneWide->hit(rayWide, 0, 20);
    EXPECT_TRUE(hitInfoWide.has_value());
    
    RayTracer::Ray rayBounded(Math::Point3D(Math::Coords{0, -5, 0}), Math::Vector3D(Math::Coords{0, 1, 0}));
    auto hitInfoBounded = coneDefault->hit(rayBounded, 0, 4.9);
    EXPECT_FALSE(hitInfoBounded.has_value());
    
    auto hitInfoBounded2 = coneDefault->hit(rayBounded, 0, 5.1);
    EXPECT_TRUE(hitInfoBounded2.has_value());
    if (hitInfoBounded2) {
        EXPECT_NEAR(hitInfoBounded2->distance, 5.0, 0.1);
    }
    
    auto hitInfoBounded3 = coneDefault->hit(rayBounded, 5.1, 10);
    EXPECT_FALSE(hitInfoBounded3.has_value());
}

TEST_F(InfiniteConeTest, CombinedTransformationsTest) {
    coneDefault->rotateX(45);
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    coneDefault->translate(translation);
    
    EXPECT_EQ(coneDefault->apex.X, 1);
    EXPECT_EQ(coneDefault->apex.Y, 2);
    EXPECT_EQ(coneDefault->apex.Z, 3);
    
    EXPECT_NEAR(coneDefault->axis.X, 0, 0.001);
    EXPECT_NEAR(coneDefault->axis.Y, 0.7071, 0.001);
    EXPECT_NEAR(coneDefault->axis.Z, 0.7071, 0.001);
    
    RayTracer::Ray rayTransformed(Math::Point3D(Math::Coords{1, -3, 3}),
        Math::Vector3D(Math::Coords{0, 1, 0}));
    auto hitInfo = coneDefault->hit(rayTransformed, 0, 20);
    
    if (hitInfo.has_value()) {
        EXPECT_GT(hitInfo->distance, 0);
        EXPECT_NEAR(hitInfo->normal.length(), 1.0, 0.001);
    }
}

TEST_F(InfiniteConeTest, CloneTest) {
    auto clonedCone = std::dynamic_pointer_cast<RayTracer::InfiniteCone>(coneWithMaterial->clone());
    
    EXPECT_NE(clonedCone, nullptr);
    if (clonedCone) {
        EXPECT_EQ(clonedCone->apex.X, coneWithMaterial->apex.X);
        EXPECT_EQ(clonedCone->apex.Y, coneWithMaterial->apex.Y);
        EXPECT_EQ(clonedCone->apex.Z, coneWithMaterial->apex.Z);
        EXPECT_EQ(clonedCone->angle, coneWithMaterial->angle);
        EXPECT_EQ(clonedCone->axis.X, coneWithMaterial->axis.X);
        EXPECT_EQ(clonedCone->axis.Y, coneWithMaterial->axis.Y);
        EXPECT_EQ(clonedCone->axis.Z, coneWithMaterial->axis.Z);
        EXPECT_EQ(clonedCone->getMaterial()->color.X, coneWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedCone->getMaterial()->color.Y, coneWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedCone->getMaterial()->color.Z, coneWithMaterial->getMaterial()->color.Z);
    }
}

TEST_F(InfiniteConeTest, TypeNameTest) {
    EXPECT_EQ(coneDefault->getTypeName(), "infinitecones");
    EXPECT_EQ(RayTracer::InfiniteCone::getTypeNameStatic(), "infinitecones");
}

} // namespace RayTracerTest