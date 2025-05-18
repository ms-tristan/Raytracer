// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for Plane primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/Plane/Plane.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class PlaneTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D position(Math::Coords{0, 0, 0});
        Math::Vector3D normal(Math::Coords{0, 1, 0});
        planeDefault = std::make_shared<RayTracer::Plane>(position, normal);

        Math::Point3D position2(Math::Coords{2, 3, 4});
        Math::Vector3D normal2(Math::Coords{1, 0, 0});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{0.5, 0.5, 0.5});
        material->reflectivity = 0.9;
        material->transparency = 0.0;
        planeWithMaterial = std::make_shared<RayTracer::Plane>(position2, normal2, material);
    }

    std::shared_ptr<RayTracer::Plane> planeDefault;
    std::shared_ptr<RayTracer::Plane> planeWithMaterial;
};

TEST_F(PlaneTest, CreationTest) {
    EXPECT_EQ(planeDefault->position.X, 0);
    EXPECT_EQ(planeDefault->position.Y, 0);
    EXPECT_EQ(planeDefault->position.Z, 0);
    EXPECT_EQ(planeDefault->normal.X, 0);
    EXPECT_EQ(planeDefault->normal.Y, 1);
    EXPECT_EQ(planeDefault->normal.Z, 0);

    EXPECT_EQ(planeWithMaterial->position.X, 2);
    EXPECT_EQ(planeWithMaterial->position.Y, 3);
    EXPECT_EQ(planeWithMaterial->position.Z, 4);
    EXPECT_EQ(planeWithMaterial->normal.X, 1);
    EXPECT_EQ(planeWithMaterial->normal.Y, 0);
    EXPECT_EQ(planeWithMaterial->normal.Z, 0);
    EXPECT_EQ(planeWithMaterial->getMaterial()->color.X, 0.5);
    EXPECT_EQ(planeWithMaterial->getMaterial()->color.Y, 0.5);
    EXPECT_EQ(planeWithMaterial->getMaterial()->color.Z, 0.5);
    EXPECT_EQ(planeWithMaterial->getMaterial()->reflectivity, 0.9);
    EXPECT_EQ(planeWithMaterial->getMaterial()->transparency, 0.0);
}

TEST_F(PlaneTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    planeDefault->translate(translation);
    
    EXPECT_EQ(planeDefault->position.X, 1);
    EXPECT_EQ(planeDefault->position.Y, 2);
    EXPECT_EQ(planeDefault->position.Z, 3);
    EXPECT_EQ(planeDefault->normal.X, 0);
    EXPECT_EQ(planeDefault->normal.Y, 1);
    EXPECT_EQ(planeDefault->normal.Z, 0);
}

TEST_F(PlaneTest, RotationTest) {
    planeDefault->rotateX(90);
    
    EXPECT_NEAR(planeDefault->normal.X, 0.0, 0.001);
    EXPECT_NEAR(planeDefault->normal.Y, 0.0, 0.001);
    EXPECT_NEAR(planeDefault->normal.Z, 1.0, 0.001);
}

TEST_F(PlaneTest, HitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{0, -5, 0}), 
                         Math::Vector3D(Math::Coords{0, 1, 0}).normalize());
    auto hitInfo = planeDefault->hit(rayHit, 0, 10);
    
    EXPECT_TRUE(hitInfo.has_value());
    if (hitInfo) {
        EXPECT_NEAR(hitInfo->distance, 5.0, 0.001);
        EXPECT_NEAR(hitInfo->normal.X, 0.0, 0.001);
        EXPECT_NEAR(std::abs(hitInfo->normal.Y), 1.0, 0.001);
        EXPECT_NEAR(hitInfo->normal.Z, 0.0, 0.001);
    }

    RayTracer::Ray rayParallel(Math::Point3D(Math::Coords{0, 1, 0}), 
                              Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoParallel = planeDefault->hit(rayParallel, 0, 10);
    
    EXPECT_FALSE(hitInfoParallel.has_value());
    
    RayTracer::Ray rayAway(Math::Point3D(Math::Coords{0, -5, 0}), 
                          Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitInfoAway = planeDefault->hit(rayAway, 0, 10);
    
    EXPECT_FALSE(hitInfoAway.has_value());
}

TEST_F(PlaneTest, CloneTest) {
    auto clonedPlane = std::dynamic_pointer_cast<RayTracer::Plane>(planeWithMaterial->clone());
    
    EXPECT_NE(clonedPlane, nullptr);
    if (clonedPlane) {
        EXPECT_EQ(clonedPlane->position.X, planeWithMaterial->position.X);
        EXPECT_EQ(clonedPlane->position.Y, planeWithMaterial->position.Y);
        EXPECT_EQ(clonedPlane->position.Z, planeWithMaterial->position.Z);
        EXPECT_EQ(clonedPlane->normal.X, planeWithMaterial->normal.X);
        EXPECT_EQ(clonedPlane->normal.Y, planeWithMaterial->normal.Y);
        EXPECT_EQ(clonedPlane->normal.Z, planeWithMaterial->normal.Z);
        EXPECT_EQ(clonedPlane->getMaterial()->color.X, planeWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedPlane->getMaterial()->color.Y, planeWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedPlane->getMaterial()->color.Z, planeWithMaterial->getMaterial()->color.Z);
        EXPECT_EQ(clonedPlane->getMaterial()->reflectivity, planeWithMaterial->getMaterial()->reflectivity);
        EXPECT_EQ(clonedPlane->getMaterial()->transparency, planeWithMaterial->getMaterial()->transparency);
    }
}

TEST_F(PlaneTest, TypeNameTest) {
    EXPECT_EQ(planeDefault->getTypeName(), "planes");
    EXPECT_EQ(RayTracer::Plane::getTypeNameStatic(), "planes");
}

TEST_F(PlaneTest, ComprehensiveHitTest) {
    RayTracer::Ray rayPerpendicular(Math::Point3D(Math::Coords{0, 10, 0}), 
                                  Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitPerpendicularInfo = planeDefault->hit(rayPerpendicular, 0, 20);
    
    EXPECT_TRUE(hitPerpendicularInfo.has_value());
    if (hitPerpendicularInfo) {
        EXPECT_NEAR(hitPerpendicularInfo->hitPoint.Y, 0.0, 0.001);
        EXPECT_NEAR(hitPerpendicularInfo->normal.Y, 1.0, 0.001);
        EXPECT_NEAR(hitPerpendicularInfo->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayFromBehind(Math::Point3D(Math::Coords{0, -10, 0}), 
                               Math::Vector3D(Math::Coords{0, 1, 0}).normalize());
    auto hitFromBehindInfo = planeDefault->hit(rayFromBehind, 0, 20);
    
    EXPECT_TRUE(hitFromBehindInfo.has_value());
    if (hitFromBehindInfo) {
        EXPECT_NEAR(hitFromBehindInfo->hitPoint.Y, 0.0, 0.001);
        EXPECT_NEAR(hitFromBehindInfo->normal.Y, -1.0, 0.001);
    }
    
    RayTracer::Ray rayAlmostParallel(Math::Point3D(Math::Coords{0, 0.1, 0}), 
                                   Math::Vector3D(Math::Coords{1, 0.01, 0}).normalize());
    auto hitAlmostParallelInfo = planeDefault->hit(rayAlmostParallel, 0, 20);
    
    if (hitAlmostParallelInfo) {
        EXPECT_GT(hitAlmostParallelInfo->distance, 0);
        EXPECT_NEAR(hitAlmostParallelInfo->normal.length(), 1.0, 0.001);
    }
}

TEST_F(PlaneTest, EdgeCaseTest) {
    RayTracer::Ray rayParallel(Math::Point3D(Math::Coords{0, 1, 0}), 
                             Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitParallelInfo = planeDefault->hit(rayParallel, 0, 20);
    
    EXPECT_FALSE(hitParallelInfo.has_value());
    
    RayTracer::Ray rayFromPlane(Math::Point3D(Math::Coords{0, 0, 0}), 
                              Math::Vector3D(Math::Coords{0, 1, 0}).normalize());
    auto hitFromPlaneInfo = planeDefault->hit(rayFromPlane, 0.0001, 20);
    
    EXPECT_FALSE(hitFromPlaneInfo.has_value());
    
    RayTracer::Ray rayNearPlane(Math::Point3D(Math::Coords{0, 0.0001, 0}), 
                              Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitNearPlaneInfo = planeDefault->hit(rayNearPlane, 0.0001, 20);
    
    EXPECT_TRUE(hitNearPlaneInfo.has_value());
    if (hitNearPlaneInfo) {
        EXPECT_NEAR(hitNearPlaneInfo->distance, 0.0001, 0.0001);
    }
}

TEST_F(PlaneTest, TransformedPlaneTest) {
    auto rotatedPlane = std::make_shared<RayTracer::Plane>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}));
    
    rotatedPlane->rotateX(45);
    rotatedPlane->rotateZ(30);
    
    RayTracer::Ray rayToRotated(Math::Point3D(Math::Coords{0, 10, 0}), 
                              Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitRotatedInfo = rotatedPlane->hit(rayToRotated, 0, 20);
    
    EXPECT_TRUE(hitRotatedInfo.has_value());
    if (hitRotatedInfo) {
        EXPECT_GT(hitRotatedInfo->distance, 0);
        EXPECT_NEAR(hitRotatedInfo->normal.length(), 1.0, 0.001);
        EXPECT_NE(hitRotatedInfo->normal.X, 0.0);
        EXPECT_NE(hitRotatedInfo->normal.Z, 0.0);
    }
    
    auto translatedPlane = std::make_shared<RayTracer::Plane>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}));
    
    translatedPlane->translate(Math::Vector3D(Math::Coords{0, 5, 0}));
    
    RayTracer::Ray rayToTranslated(Math::Point3D(Math::Coords{0, 10, 0}), 
                                 Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitTranslatedInfo = translatedPlane->hit(rayToTranslated, 0, 20);
    
    EXPECT_TRUE(hitTranslatedInfo.has_value());
    if (hitTranslatedInfo) {
        EXPECT_NEAR(hitTranslatedInfo->hitPoint.Y, 5.0, 0.001);
    }
}

} // namespace RayTracerTest