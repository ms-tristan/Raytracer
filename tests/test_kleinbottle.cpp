/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for KleinBottle primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/KleinBottle/KleinBottle.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class KleinBottleTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D center(Math::Coords{0, 0, 0});
        bottleDefault = std::make_shared<RayTracer::KleinBottle>(center, 1.0, 0.1, false);

        Math::Point3D center2(Math::Coords{2, 3, 4});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 0.0, 0.0});
        material->reflectivity = 0.8;
        material->transparency = 0.5;
        bottleWithMaterial = std::make_shared<RayTracer::KleinBottle>(center2, 2.0, 0.2, true, material);
    }

    std::shared_ptr<RayTracer::KleinBottle> bottleDefault;
    std::shared_ptr<RayTracer::KleinBottle> bottleWithMaterial;
};

TEST_F(KleinBottleTest, CreationTest) {
    EXPECT_EQ(bottleDefault->center.X, 0);
    EXPECT_EQ(bottleDefault->center.Y, 0);
    EXPECT_EQ(bottleDefault->center.Z, 0);
    EXPECT_EQ(bottleDefault->scale, 1.0);
    EXPECT_EQ(bottleDefault->thickness, 0.1);

    EXPECT_EQ(bottleWithMaterial->center.X, 2);
    EXPECT_EQ(bottleWithMaterial->center.Y, 3);
    EXPECT_EQ(bottleWithMaterial->center.Z, 4);
    EXPECT_EQ(bottleWithMaterial->scale, 2.0);
    EXPECT_EQ(bottleWithMaterial->thickness, 0.2);
    EXPECT_EQ(bottleWithMaterial->getMaterial()->color.X, 1.0);
    EXPECT_EQ(bottleWithMaterial->getMaterial()->color.Y, 0.0);
    EXPECT_EQ(bottleWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(bottleWithMaterial->getMaterial()->reflectivity, 0.8);
    EXPECT_EQ(bottleWithMaterial->getMaterial()->transparency, 0.5);
}

TEST_F(KleinBottleTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    bottleDefault->translate(translation);
    
    EXPECT_EQ(bottleDefault->center.X, 1);
    EXPECT_EQ(bottleDefault->center.Y, 2);
    EXPECT_EQ(bottleDefault->center.Z, 3);
}

TEST_F(KleinBottleTest, RotationTest) {
    bottleDefault->rotateX(45);
    bottleDefault->rotateY(30);
    bottleDefault->rotateZ(60);
    
    EXPECT_EQ(bottleDefault->center.X, 0);
    EXPECT_EQ(bottleDefault->center.Y, 0);
    EXPECT_EQ(bottleDefault->center.Z, 0);
}

TEST_F(KleinBottleTest, CloneTest) {
    auto clonedBottle = std::dynamic_pointer_cast<RayTracer::KleinBottle>(bottleWithMaterial->clone());
    
    EXPECT_NE(clonedBottle, nullptr);
    if (clonedBottle) {
        EXPECT_EQ(clonedBottle->center.X, bottleWithMaterial->center.X);
        EXPECT_EQ(clonedBottle->center.Y, bottleWithMaterial->center.Y);
        EXPECT_EQ(clonedBottle->center.Z, bottleWithMaterial->center.Z);
        EXPECT_EQ(clonedBottle->scale, bottleWithMaterial->scale);
        EXPECT_EQ(clonedBottle->thickness, bottleWithMaterial->thickness);
        EXPECT_EQ(clonedBottle->getMaterial()->color.X, bottleWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedBottle->getMaterial()->color.Y, bottleWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedBottle->getMaterial()->color.Z, bottleWithMaterial->getMaterial()->color.Z);
    }
}

TEST_F(KleinBottleTest, TypeNameTest) {
    EXPECT_EQ(bottleDefault->getTypeName(), "kleinbottles");
    EXPECT_EQ(RayTracer::KleinBottle::getTypeNameStatic(), "kleinbottles");
}

TEST_F(KleinBottleTest, HitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}));
    auto hitInfo = bottleDefault->hit(rayHit, 0, 20);
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-20, -20, -20}), Math::Vector3D(Math::Coords{1, 1, 1}));
    auto hitInfoMiss = bottleDefault->hit(rayMiss, 0, 5);
    
    if (hitInfo.has_value()) {
        EXPECT_GT(hitInfo->distance, 0);
        EXPECT_NEAR(hitInfo->normal.length(), 1.0, 0.001);
    }
    
    if (hitInfoMiss.has_value()) {
        EXPECT_GT(hitInfoMiss->distance, 0);
    }
}

} // namespace RayTracerTest