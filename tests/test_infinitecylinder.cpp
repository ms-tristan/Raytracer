/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for InfiniteCylinder primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/InfiniteCylinder/InfiniteCylinder.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class InfiniteCylinderTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D center(Math::Coords{0, 0, 0});
        Math::Vector3D axis(Math::Coords{0, 1, 0});
        cylinderDefault = std::make_shared<RayTracer::InfiniteCylinder>(center, axis, 1.0);

        Math::Point3D center2(Math::Coords{2, 3, 4});
        Math::Vector3D axis2(Math::Coords{1, 0, 0});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 0.0, 0.0});
        material->reflectivity = 0.8;
        material->transparency = 0.5;
        cylinderWithMaterial = std::make_shared<RayTracer::InfiniteCylinder>(center2, axis2, 2.0, material);
    }

    std::shared_ptr<RayTracer::InfiniteCylinder> cylinderDefault;
    std::shared_ptr<RayTracer::InfiniteCylinder> cylinderWithMaterial;
};

TEST_F(InfiniteCylinderTest, CreationTest) {
    EXPECT_EQ(cylinderDefault->center.X, 0);
    EXPECT_EQ(cylinderDefault->center.Y, 0);
    EXPECT_EQ(cylinderDefault->center.Z, 0);
    EXPECT_EQ(cylinderDefault->radius, 1.0);
    EXPECT_EQ(cylinderDefault->axis.X, 0);
    EXPECT_EQ(cylinderDefault->axis.Y, 1);
    EXPECT_EQ(cylinderDefault->axis.Z, 0);

    EXPECT_EQ(cylinderWithMaterial->center.X, 2);
    EXPECT_EQ(cylinderWithMaterial->center.Y, 3);
    EXPECT_EQ(cylinderWithMaterial->center.Z, 4);
    EXPECT_EQ(cylinderWithMaterial->radius, 2.0);
    EXPECT_EQ(cylinderWithMaterial->axis.X, 1);
    EXPECT_EQ(cylinderWithMaterial->axis.Y, 0);
    EXPECT_EQ(cylinderWithMaterial->axis.Z, 0);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->color.X, 1.0);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->color.Y, 0.0);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->reflectivity, 0.8);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->transparency, 0.5);
}

TEST_F(InfiniteCylinderTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    cylinderDefault->translate(translation);
    
    EXPECT_EQ(cylinderDefault->center.X, 1);
    EXPECT_EQ(cylinderDefault->center.Y, 2);
    EXPECT_EQ(cylinderDefault->center.Z, 3);
    
    EXPECT_EQ(cylinderDefault->axis.X, 0);
    EXPECT_EQ(cylinderDefault->axis.Y, 1);
    EXPECT_EQ(cylinderDefault->axis.Z, 0);
}

TEST_F(InfiniteCylinderTest, RotationTest) {
    cylinderDefault->rotateX(90);
    
    EXPECT_NEAR(cylinderDefault->axis.X, 0, 0.001);
    EXPECT_NEAR(cylinderDefault->axis.Y, 0, 0.001);
    EXPECT_NEAR(cylinderDefault->axis.Z, 1, 0.001);
    
    cylinderDefault->rotateY(90);
    
    EXPECT_NEAR(cylinderDefault->axis.X, 1, 0.001);
    EXPECT_NEAR(cylinderDefault->axis.Y, 0, 0.001);
    EXPECT_NEAR(cylinderDefault->axis.Z, 0, 0.001);
}

TEST_F(InfiniteCylinderTest, HitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}));
    auto hitInfo = cylinderDefault->hit(rayHit, 0, 10);
    
    EXPECT_TRUE(hitInfo.has_value());
    if (hitInfo) {
        EXPECT_NEAR(hitInfo->distance, 4.0, 0.001);
        EXPECT_NEAR(hitInfo->hitPoint.X, -1.0, 0.001);
        EXPECT_NEAR(hitInfo->hitPoint.Y, 0.0, 0.001);
        EXPECT_NEAR(hitInfo->hitPoint.Z, 0.0, 0.001);
        EXPECT_NEAR(hitInfo->normal.X, -1.0, 0.001);
        EXPECT_NEAR(hitInfo->normal.Y, 0.0, 0.001);
        EXPECT_NEAR(hitInfo->normal.Z, 0.0, 0.001);
    }
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, 0, 2}), Math::Vector3D(Math::Coords{1, 0, 0}));
    auto hitInfoMiss = cylinderDefault->hit(rayMiss, 0, 10);
    
    EXPECT_FALSE(hitInfoMiss.has_value());
    
    RayTracer::Ray rayAngled(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0.5, 0.1}).normalize());
    auto hitInfoAngled = cylinderDefault->hit(rayAngled, 0, 10);
    
    EXPECT_TRUE(hitInfoAngled.has_value());
    if (hitInfoAngled) {
        EXPECT_GT(hitInfoAngled->distance, 0);
    }
}

TEST_F(InfiniteCylinderTest, CloneTest) {
    auto clonedCylinder = std::dynamic_pointer_cast<RayTracer::InfiniteCylinder>(cylinderWithMaterial->clone());
    
    EXPECT_NE(clonedCylinder, nullptr);
    if (clonedCylinder) {
        EXPECT_EQ(clonedCylinder->center.X, cylinderWithMaterial->center.X);
        EXPECT_EQ(clonedCylinder->center.Y, cylinderWithMaterial->center.Y);
        EXPECT_EQ(clonedCylinder->center.Z, cylinderWithMaterial->center.Z);
        EXPECT_EQ(clonedCylinder->radius, cylinderWithMaterial->radius);
        EXPECT_EQ(clonedCylinder->axis.X, cylinderWithMaterial->axis.X);
        EXPECT_EQ(clonedCylinder->axis.Y, cylinderWithMaterial->axis.Y);
        EXPECT_EQ(clonedCylinder->axis.Z, cylinderWithMaterial->axis.Z);
        EXPECT_EQ(clonedCylinder->getMaterial()->color.X, cylinderWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedCylinder->getMaterial()->color.Y, cylinderWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedCylinder->getMaterial()->color.Z, cylinderWithMaterial->getMaterial()->color.Z);
    }
}

TEST_F(InfiniteCylinderTest, TypeNameTest) {
    EXPECT_EQ(cylinderDefault->getTypeName(), "infinitecylinders");
    EXPECT_EQ(RayTracer::InfiniteCylinder::getTypeNameStatic(), "infinitecylinders");
}

TEST_F(InfiniteCylinderTest, EdgeCaseTest) {
    RayTracer::Ray rayAlmostParallel(Math::Point3D(Math::Coords{0.1, -5, 0}), 
                                   Math::Vector3D(Math::Coords{0.01, 1, 0}).normalize());
    auto hitInfoAlmostParallel = cylinderDefault->hit(rayAlmostParallel, 0, 20);
    
    if (hitInfoAlmostParallel) {
        EXPECT_GT(hitInfoAlmostParallel->distance, 0);
        EXPECT_NEAR(hitInfoAlmostParallel->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayTangent(Math::Point3D(Math::Coords{-5, 0, 1}), 
                            Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoTangent = cylinderDefault->hit(rayTangent, 0, 20);
    
    if (hitInfoTangent) {
        EXPECT_GT(hitInfoTangent->distance, 0);
        EXPECT_NEAR(hitInfoTangent->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayAlmostMiss(Math::Point3D(Math::Coords{-5, 0, 1.01}), 
                               Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoAlmostMiss = cylinderDefault->hit(rayAlmostMiss, 0, 20);
    
    if (!hitInfoAlmostMiss) {
        EXPECT_FALSE(hitInfoAlmostMiss.has_value());
    }
}

TEST_F(InfiniteCylinderTest, BoundsTest) {
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitTooSmall = cylinderDefault->hit(ray, 0, 3);
    EXPECT_FALSE(hitTooSmall.has_value());
    
    auto hitNormal = cylinderDefault->hit(ray, 0, 10);
    if (hitNormal.has_value()) {
        double actualDistance = hitNormal->distance;
        auto hitAfterMin = cylinderDefault->hit(ray, actualDistance + 0.1, 10);
        
        if (hitAfterMin.has_value()) {
            EXPECT_GT(hitAfterMin->distance, actualDistance);
        }
    }
}

TEST_F(InfiniteCylinderTest, ComprehensiveHitTest) {
    RayTracer::Ray rayPerpendicular(Math::Point3D(Math::Coords{-5, 0, 0}), 
                                  Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoPerpendicular = cylinderDefault->hit(rayPerpendicular, 0, 20);
    
    EXPECT_TRUE(hitInfoPerpendicular.has_value());
    if (hitInfoPerpendicular) {
        EXPECT_NEAR(hitInfoPerpendicular->hitPoint.X, -1.0, 0.001);
        EXPECT_NEAR(hitInfoPerpendicular->normal.Y, 0.0, 0.001);
    }
    
    RayTracer::Ray rayOblique(Math::Point3D(Math::Coords{-5, -5, 0}), 
                            Math::Vector3D(Math::Coords{1, 1, 0}).normalize());
    auto hitInfoOblique = cylinderDefault->hit(rayOblique, 0, 20);
    
    EXPECT_TRUE(hitInfoOblique.has_value());
    if (hitInfoOblique) {
        EXPECT_GT(hitInfoOblique->distance, 0);
        EXPECT_NEAR(hitInfoOblique->normal.dot(cylinderDefault->axis), 0.0, 0.001);
    }
}

TEST_F(InfiniteCylinderTest, TransformedCylinderTest) {
    auto rotatedCylinder = std::make_shared<RayTracer::InfiniteCylinder>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        1.0);
    
    rotatedCylinder->rotateX(45);
    rotatedCylinder->rotateZ(30);
    
    RayTracer::Ray rayToRotated(Math::Point3D(Math::Coords{-5, 0, 0}), 
                              Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitRotatedInfo = rotatedCylinder->hit(rayToRotated, 0, 20);
    
    if (hitRotatedInfo) {
        EXPECT_GT(hitRotatedInfo->distance, 0);
        EXPECT_NEAR(hitRotatedInfo->normal.length(), 1.0, 0.001);
        EXPECT_NEAR(hitRotatedInfo->normal.dot(rotatedCylinder->axis), 0.0, 0.001);
    }
    
    auto translatedCylinder = std::make_shared<RayTracer::InfiniteCylinder>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        1.0);
    
    translatedCylinder->translate(Math::Vector3D(Math::Coords{3, 0, 0}));
    
    auto hitTranslatedInfo = translatedCylinder->hit(rayToRotated, 0, 20);
    
    EXPECT_TRUE(hitTranslatedInfo.has_value());
    if (hitTranslatedInfo) {
        EXPECT_NEAR(hitTranslatedInfo->hitPoint.X, 2.0, 0.001);
    }
}

TEST_F(InfiniteCylinderTest, ScaleTest) {
    auto thinCylinder = std::make_shared<RayTracer::InfiniteCylinder>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        0.5);
        
    auto wideCylinder = std::make_shared<RayTracer::InfiniteCylinder>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        2.0);
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitThin = thinCylinder->hit(ray, 0, 20);
    auto hitWide = wideCylinder->hit(ray, 0, 20);
    
    EXPECT_TRUE(hitThin.has_value());
    EXPECT_TRUE(hitWide.has_value());
    
    if (hitThin.has_value() && hitWide.has_value()) {
        EXPECT_LT(hitWide->distance, hitThin->distance);
        
        EXPECT_NEAR(hitThin->hitPoint.X, -0.5, 0.001);
        EXPECT_NEAR(hitWide->hitPoint.X, -2.0, 0.001);
    }
}

} // namespace RayTracerTest