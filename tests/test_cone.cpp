/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for Cone primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/Cone/Cone.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class ConeTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D apex(Math::Coords{0, 0, 0});
        Math::Vector3D axis(Math::Coords{0, 1, 0});
        coneDefault = std::make_shared<RayTracer::Cone>(apex, axis, 1.0, 2.0);

        Math::Point3D apex2(Math::Coords{2, 3, 4});
        Math::Vector3D axis2(Math::Coords{1, 0, 0});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 1.0, 0.0});
        material->reflectivity = 0.4;
        material->transparency = 0.1;
        coneWithMaterial = std::make_shared<RayTracer::Cone>(apex2, axis2, 2.0, 4.0, material);
    }

    std::shared_ptr<RayTracer::Cone> coneDefault;
    std::shared_ptr<RayTracer::Cone> coneWithMaterial;
};

TEST_F(ConeTest, CreationTest) {
    EXPECT_EQ(coneDefault->apex.X, 0);
    EXPECT_EQ(coneDefault->apex.Y, 0);
    EXPECT_EQ(coneDefault->apex.Z, 0);
    EXPECT_EQ(coneDefault->axis.X, 0);
    EXPECT_EQ(coneDefault->axis.Y, 1);
    EXPECT_EQ(coneDefault->axis.Z, 0);
    EXPECT_EQ(coneDefault->radius, 1.0);
    EXPECT_EQ(coneDefault->height, 2.0);

    EXPECT_EQ(coneWithMaterial->apex.X, 2);
    EXPECT_EQ(coneWithMaterial->apex.Y, 3);
    EXPECT_EQ(coneWithMaterial->apex.Z, 4);
    EXPECT_EQ(coneWithMaterial->axis.X, 1);
    EXPECT_EQ(coneWithMaterial->axis.Y, 0);
    EXPECT_EQ(coneWithMaterial->axis.Z, 0);
    EXPECT_EQ(coneWithMaterial->radius, 2.0);
    EXPECT_EQ(coneWithMaterial->height, 4.0);
    EXPECT_EQ(coneWithMaterial->getMaterial()->color.X, 1.0);
    EXPECT_EQ(coneWithMaterial->getMaterial()->color.Y, 1.0);
    EXPECT_EQ(coneWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(coneWithMaterial->getMaterial()->reflectivity, 0.4);
    EXPECT_EQ(coneWithMaterial->getMaterial()->transparency, 0.1);
}

TEST_F(ConeTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    coneDefault->translate(translation);
    
    EXPECT_EQ(coneDefault->apex.X, 1);
    EXPECT_EQ(coneDefault->apex.Y, 2);
    EXPECT_EQ(coneDefault->apex.Z, 3);
}

TEST_F(ConeTest, RotationTest) {
    coneDefault->rotateX(90);
    
    EXPECT_NEAR(coneDefault->axis.X, 0.0, 0.001);
    EXPECT_NEAR(coneDefault->axis.Y, 0.0, 0.001);
    EXPECT_NEAR(coneDefault->axis.Z, 1.0, 0.001);
}

TEST_F(ConeTest, HitTest) {
    RayTracer::Ray raySide(Math::Point3D(Math::Coords{-3, 1, 0}), 
                          Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoSide = coneDefault->hit(raySide, 0, 10);
    
    EXPECT_TRUE(hitInfoSide.has_value());

    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-3, 3, 0}), 
                          Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoMiss = coneDefault->hit(rayMiss, 0, 10);
    
    EXPECT_FALSE(hitInfoMiss.has_value());
}

TEST_F(ConeTest, CloneTest) {
    auto clonedCone = std::dynamic_pointer_cast<RayTracer::Cone>(coneWithMaterial->clone());
    
    EXPECT_NE(clonedCone, nullptr);
    if (clonedCone) {
        EXPECT_EQ(clonedCone->apex.X, coneWithMaterial->apex.X);
        EXPECT_EQ(clonedCone->apex.Y, coneWithMaterial->apex.Y);
        EXPECT_EQ(clonedCone->apex.Z, coneWithMaterial->apex.Z);
        EXPECT_EQ(clonedCone->axis.X, coneWithMaterial->axis.X);
        EXPECT_EQ(clonedCone->axis.Y, coneWithMaterial->axis.Y);
        EXPECT_EQ(clonedCone->axis.Z, coneWithMaterial->axis.Z);
        EXPECT_EQ(clonedCone->radius, coneWithMaterial->radius);
        EXPECT_EQ(clonedCone->height, coneWithMaterial->height);
        EXPECT_EQ(clonedCone->getMaterial()->color.X, coneWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedCone->getMaterial()->color.Y, coneWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedCone->getMaterial()->color.Z, coneWithMaterial->getMaterial()->color.Z);
        EXPECT_EQ(clonedCone->getMaterial()->reflectivity, coneWithMaterial->getMaterial()->reflectivity);
        EXPECT_EQ(clonedCone->getMaterial()->transparency, coneWithMaterial->getMaterial()->transparency);
    }
}

TEST_F(ConeTest, TypeNameTest) {
    EXPECT_EQ(coneDefault->getTypeName(), "cones");
    EXPECT_EQ(RayTracer::Cone::getTypeNameStatic(), "cones");
}

TEST_F(ConeTest, ComprehensiveHitTest) {
    RayTracer::Ray raySide(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitSide = coneDefault->hit(raySide, 0, 20);
    if (hitSide) {
        EXPECT_GT(hitSide->distance, 0);
        EXPECT_NEAR(hitSide->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayBase(Math::Point3D(Math::Coords{0, -5, 0}), Math::Vector3D(Math::Coords{0, 1, 0}).normalize());
    auto hitBase = coneDefault->hit(rayBase, 0, 20);
    if (hitBase) {
        EXPECT_NEAR(hitBase->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayApex(Math::Point3D(Math::Coords{0, 5, 0}), Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitApex = coneDefault->hit(rayApex, 0, 20);
    if (hitApex) {
        EXPECT_GT(hitApex->distance, 0);
        EXPECT_NEAR(hitApex->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, 0, 5}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitMiss = coneDefault->hit(rayMiss, 0, 20);
}

TEST_F(ConeTest, InsideOutsideTest) {
    RayTracer::Ray rayFromCenter(Math::Point3D(Math::Coords{0, 0.1, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitFromCenter = coneDefault->hit(rayFromCenter, 0, 20);
    if (hitFromCenter) {
        EXPECT_GT(hitFromCenter->distance, 0);
        EXPECT_NEAR(hitFromCenter->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayToBase(Math::Point3D(Math::Coords{0, 0.1, 0}), Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitToBase = coneDefault->hit(rayToBase, 0, 20);
    if (hitToBase) {
        EXPECT_GT(hitToBase->distance, 0);
        EXPECT_NEAR(hitToBase->normal.length(), 1.0, 0.001);
    }
}

TEST_F(ConeTest, EdgeCaseTests) {
    RayTracer::Ray rayTangent(Math::Point3D(Math::Coords{-5, -5, 0}), Math::Vector3D(Math::Coords{1, 1, 0}).normalize());
    auto hitTangent = coneDefault->hit(rayTangent, 0, 20);
    
    Math::Point3D center(Math::Coords{0, 0, 0});
    Math::Vector3D axis(Math::Coords{0, 1, 0});
    auto sharpCone = std::make_shared<RayTracer::Cone>(center, axis, 0.1, 1.0);
    
    RayTracer::Ray rayToSharp(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitSharp = sharpCone->hit(rayToSharp, 0, 20);
    if (hitSharp.has_value()) {
        EXPECT_GT(hitSharp->distance, 0);
    }
    
    auto wideCone = std::make_shared<RayTracer::Cone>(center, axis, 5.0, 1.0);
    
    RayTracer::Ray rayToWide(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitWide = wideCone->hit(rayToWide, 0, 20);
    EXPECT_TRUE(hitWide.has_value());
}

TEST_F(ConeTest, TransformedConeTest) {
    auto translatedCone = std::make_shared<RayTracer::Cone>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        1.0, 2.0);
    translatedCone->translate(Math::Vector3D(Math::Coords{5, 0, 0}));
    
    RayTracer::Ray rayToTranslated(Math::Point3D(Math::Coords{10, 0, 0}), Math::Vector3D(Math::Coords{-1, 0, 0}).normalize());
    auto hitTranslated = translatedCone->hit(rayToTranslated, 0, 20);
    auto hitDefault = coneDefault->hit(rayToTranslated, 0, 20);
    
    if (hitTranslated.has_value()) {
        EXPECT_GT(hitTranslated->distance, 0);
        EXPECT_NEAR(hitTranslated->normal.length(), 1.0, 0.001);
    }
    
    if (hitDefault.has_value()) {
        EXPECT_GT(hitDefault->distance, 0);
        EXPECT_NEAR(hitDefault->normal.length(), 1.0, 0.001);
    }
    
    auto rotatedCone = std::make_shared<RayTracer::Cone>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        1.0, 2.0);
    rotatedCone->rotateX(45);
    rotatedCone->rotateY(30);
    rotatedCone->rotateZ(60);
    
    RayTracer::Ray rayToRotated(Math::Point3D(Math::Coords{-5, -5, -5}), Math::Vector3D(Math::Coords{1, 1, 1}).normalize());
    auto hitRotated = rotatedCone->hit(rayToRotated, 0, 20);
    if (hitRotated.has_value()) {
        EXPECT_GT(hitRotated->distance, 0);
        EXPECT_NEAR(hitRotated->normal.length(), 1.0, 0.001);
    }
}

TEST_F(ConeTest, BoundsTest) {
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitTooSmall = coneDefault->hit(ray, 0, 3);
    if (hitTooSmall.has_value()) {
        EXPECT_LT(hitTooSmall->distance, 3);
    }
    
    auto hitAtMax = coneDefault->hit(ray, 0, 20);
    
    auto hit = coneDefault->hit(ray, 0, 20);
    if (hit.has_value()) {
        double actualDistance = hit->distance;
        auto hitAfterMin = coneDefault->hit(ray, actualDistance + 0.1, 10);
    }
}

TEST_F(ConeTest, DifferentSizeTest) {
    auto smallCone = std::make_shared<RayTracer::Cone>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        0.5, 0.5);
    
    auto largeCone = std::make_shared<RayTracer::Cone>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        4.0, 8.0);
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitSmall = smallCone->hit(ray, 0, 20);
    auto hitLarge = largeCone->hit(ray, 0, 20);
    
    if (hitLarge.has_value()) {
        EXPECT_GT(hitLarge->distance, 0);
        EXPECT_NEAR(hitLarge->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayHighY(Math::Point3D(Math::Coords{-5, 3, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitSmallHighY = smallCone->hit(rayHighY, 0, 20);
    auto hitLargeHighY = largeCone->hit(rayHighY, 0, 20);
    
    if (hitLargeHighY.has_value()) {
        EXPECT_GT(hitLargeHighY->distance, 0);
        EXPECT_NEAR(hitLargeHighY->normal.length(), 1.0, 0.001);
    }
}

} // namespace RayTracerTest