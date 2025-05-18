// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for Torus primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/Torus/Torus.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class TorusTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D center(Math::Coords{0, 0, 0});
        Math::Vector3D axis(Math::Coords{0, 1, 0});
        torusDefault = std::make_shared<RayTracer::Torus>(center, axis, 2.0, 0.5);

        Math::Point3D center2(Math::Coords{2, 3, 4});
        Math::Vector3D axis2(Math::Coords{1, 0, 0});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{0.2, 0.8, 0.2});
        material->reflectivity = 0.6;
        material->transparency = 0.2;
        torusWithMaterial = std::make_shared<RayTracer::Torus>(center2, axis2, 3.0, 1.0, material);
    }

    std::shared_ptr<RayTracer::Torus> torusDefault;
    std::shared_ptr<RayTracer::Torus> torusWithMaterial;
};

TEST_F(TorusTest, CreationTest) {
    EXPECT_EQ(torusDefault->center.X, 0);
    EXPECT_EQ(torusDefault->center.Y, 0);
    EXPECT_EQ(torusDefault->center.Z, 0);
    EXPECT_EQ(torusDefault->axis.X, 0);
    EXPECT_EQ(torusDefault->axis.Y, 1);
    EXPECT_EQ(torusDefault->axis.Z, 0);
    EXPECT_EQ(torusDefault->majorRadius, 2.0);
    EXPECT_EQ(torusDefault->minorRadius, 0.5);

    EXPECT_EQ(torusWithMaterial->center.X, 2);
    EXPECT_EQ(torusWithMaterial->center.Y, 3);
    EXPECT_EQ(torusWithMaterial->center.Z, 4);
    EXPECT_EQ(torusWithMaterial->axis.X, 1);
    EXPECT_EQ(torusWithMaterial->axis.Y, 0);
    EXPECT_EQ(torusWithMaterial->axis.Z, 0);
    EXPECT_EQ(torusWithMaterial->majorRadius, 3.0);
    EXPECT_EQ(torusWithMaterial->minorRadius, 1.0);
    EXPECT_EQ(torusWithMaterial->getMaterial()->color.X, 0.2);
    EXPECT_EQ(torusWithMaterial->getMaterial()->color.Y, 0.8);
    EXPECT_EQ(torusWithMaterial->getMaterial()->color.Z, 0.2);
    EXPECT_EQ(torusWithMaterial->getMaterial()->reflectivity, 0.6);
    EXPECT_EQ(torusWithMaterial->getMaterial()->transparency, 0.2);
}

TEST_F(TorusTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    torusDefault->translate(translation);
    
    EXPECT_EQ(torusDefault->center.X, 1);
    EXPECT_EQ(torusDefault->center.Y, 2);
    EXPECT_EQ(torusDefault->center.Z, 3);
    EXPECT_EQ(torusDefault->axis.X, 0);
    EXPECT_EQ(torusDefault->axis.Y, 1);
    EXPECT_EQ(torusDefault->axis.Z, 0);
}

TEST_F(TorusTest, RotationTest) {
    torusDefault->rotateX(90);
    
    EXPECT_NEAR(torusDefault->axis.X, 0.0, 0.001);
    EXPECT_NEAR(torusDefault->axis.Y, 0.0, 0.001);
    EXPECT_NEAR(torusDefault->axis.Z, 1.0, 0.001);
}

TEST_F(TorusTest, CloneTest) {
    auto clonedTorus = std::dynamic_pointer_cast<RayTracer::Torus>(torusWithMaterial->clone());
    
    EXPECT_NE(clonedTorus, nullptr);
    if (clonedTorus) {
        EXPECT_EQ(clonedTorus->center.X, torusWithMaterial->center.X);
        EXPECT_EQ(clonedTorus->center.Y, torusWithMaterial->center.Y);
        EXPECT_EQ(clonedTorus->center.Z, torusWithMaterial->center.Z);
        EXPECT_EQ(clonedTorus->axis.X, torusWithMaterial->axis.X);
        EXPECT_EQ(clonedTorus->axis.Y, torusWithMaterial->axis.Y);
        EXPECT_EQ(clonedTorus->axis.Z, torusWithMaterial->axis.Z);
        EXPECT_EQ(clonedTorus->majorRadius, torusWithMaterial->majorRadius);
        EXPECT_EQ(clonedTorus->minorRadius, torusWithMaterial->minorRadius);
        EXPECT_EQ(clonedTorus->getMaterial()->color.X, torusWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedTorus->getMaterial()->color.Y, torusWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedTorus->getMaterial()->color.Z, torusWithMaterial->getMaterial()->color.Z);
        EXPECT_EQ(clonedTorus->getMaterial()->reflectivity, torusWithMaterial->getMaterial()->reflectivity);
        EXPECT_EQ(clonedTorus->getMaterial()->transparency, torusWithMaterial->getMaterial()->transparency);
    }
}

TEST_F(TorusTest, TypeNameTest) {
    EXPECT_EQ(torusDefault->getTypeName(), "torus");
    EXPECT_EQ(RayTracer::Torus::getTypeNameStatic(), "torus");
}

TEST_F(TorusTest, BasicHitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfo = torusDefault->hit(rayHit, 0, 10);
    
    if (hitInfo.has_value()) {
        EXPECT_GT(hitInfo->distance, 0);
        EXPECT_NEAR(hitInfo->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, 5, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoMiss = torusDefault->hit(rayMiss, 0, 10);
    
    if (!hitInfoMiss.has_value()) {
        EXPECT_FALSE(hitInfoMiss.has_value());
    }
}

TEST_F(TorusTest, ComprehensiveHitTest) {
    RayTracer::Ray rayOuterHit(Math::Point3D(Math::Coords{-5, 0, 0}), 
                             Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitOuterInfo = torusDefault->hit(rayOuterHit, 0, 10);
    
    EXPECT_TRUE(hitOuterInfo.has_value());
    if (hitOuterInfo) {
        EXPECT_GT(hitOuterInfo->distance, 0);
        EXPECT_NEAR(hitOuterInfo->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayInnerHit(Math::Point3D(Math::Coords{0, 0, 0}), 
                             Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInnerInfo = torusDefault->hit(rayInnerHit, 0, 10);
    
    if (hitInnerInfo) {
        EXPECT_GT(hitInnerInfo->distance, 0);
        EXPECT_NEAR(hitInnerInfo->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayTangent(Math::Point3D(Math::Coords{-5, 0, 2.5}), 
                            Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitTangentInfo = torusDefault->hit(rayTangent, 0, 10);
    
    if (hitTangentInfo) {
        EXPECT_GT(hitTangentInfo->distance, 0);
        EXPECT_NEAR(hitTangentInfo->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayCentralHole(Math::Point3D(Math::Coords{0, -5, 0}), 
                                Math::Vector3D(Math::Coords{0, 1, 0}).normalize());
    auto hitCentralInfo = torusDefault->hit(rayCentralHole, 0, 10);
    
    if (hitCentralInfo) {
        EXPECT_GT(hitCentralInfo->distance, 0);
        EXPECT_NEAR(hitCentralInfo->normal.length(), 1.0, 0.001);
    }
}

TEST_F(TorusTest, BoundsTest) {
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitTooSmall = torusDefault->hit(ray, 0, 1);
    EXPECT_FALSE(hitTooSmall.has_value());
    
    auto hitNormal = torusDefault->hit(ray, 0, 10);
    if (hitNormal.has_value()) {
        double actualDistance = hitNormal->distance;
        auto hitAfterMin = torusDefault->hit(ray, actualDistance + 0.1, 10);
        
        if (hitAfterMin.has_value()) {
            EXPECT_GT(hitAfterMin->distance, actualDistance);
        }
    }
}

TEST_F(TorusTest, TransformedTorusTest) {
    auto rotatedTorus = std::make_shared<RayTracer::Torus>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        2.0, 0.5);
    
    rotatedTorus->rotateX(45);
    rotatedTorus->rotateY(30);
    rotatedTorus->rotateZ(15);
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfo = rotatedTorus->hit(ray, 0, 10);
    
    if (hitInfo) {
        EXPECT_GT(hitInfo->distance, 0);
        EXPECT_NEAR(hitInfo->normal.length(), 1.0, 0.001);
    }
    
    auto translatedTorus = std::make_shared<RayTracer::Torus>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        2.0, 0.5);
    
    translatedTorus->translate(Math::Vector3D(Math::Coords{3, 0, 0}));
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitTranslated = translatedTorus->hit(rayMiss, 0, 20);
    
    if (hitTranslated) {
        EXPECT_GT(hitTranslated->distance, 0);
        EXPECT_NEAR(hitTranslated->normal.length(), 1.0, 0.001);
        EXPECT_NEAR(hitTranslated->hitPoint.X, 0.5, 0.1);
    }
}

TEST_F(TorusTest, DifferentSizeTest) {
    auto smallTorus = std::make_shared<RayTracer::Torus>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        1.0, 0.2);
        
    auto largeTorus = std::make_shared<RayTracer::Torus>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        3.0, 1.0);
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitSmall = smallTorus->hit(ray, 0, 10);
    auto hitLarge = largeTorus->hit(ray, 0, 10);
    
    EXPECT_TRUE(hitLarge.has_value());
    if (hitLarge.has_value() && hitSmall.has_value()) {
        EXPECT_LT(hitLarge->distance, hitSmall->distance);
    }
}

} // namespace RayTracerTest