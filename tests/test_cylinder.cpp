/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for Cylinder primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/Cylinder/Cylinder.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class CylinderTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D center1(Math::Coords{0, 0, 0});
        Math::Vector3D axis1(Math::Coords{0, 1, 0});
        cylinderDefault = std::make_shared<RayTracer::Cylinder>(center1, axis1, 1.0, 2.0);

        Math::Point3D center2(Math::Coords{2, 3, 4});
        Math::Vector3D axis2(Math::Coords{1, 0, 0});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 0.0, 0.0});
        material->reflectivity = 0.8;
        material->transparency = 0.5;
        cylinderWithMaterial = std::make_shared<RayTracer::Cylinder>(center2, axis2, 2.0, 4.0, material);
    }

    std::shared_ptr<RayTracer::Cylinder> cylinderDefault;
    std::shared_ptr<RayTracer::Cylinder> cylinderWithMaterial;
};

TEST_F(CylinderTest, CreationTest) {
    EXPECT_EQ(cylinderDefault->center.X, 0);
    EXPECT_EQ(cylinderDefault->center.Y, 0);
    EXPECT_EQ(cylinderDefault->center.Z, 0);
    EXPECT_EQ(cylinderDefault->radius, 1.0);
    EXPECT_EQ(cylinderDefault->height, 2.0);
    EXPECT_EQ(cylinderDefault->axis.X, 0);
    EXPECT_EQ(cylinderDefault->axis.Y, 1);
    EXPECT_EQ(cylinderDefault->axis.Z, 0);

    EXPECT_EQ(cylinderWithMaterial->center.X, 2);
    EXPECT_EQ(cylinderWithMaterial->center.Y, 3);
    EXPECT_EQ(cylinderWithMaterial->center.Z, 4);
    EXPECT_EQ(cylinderWithMaterial->radius, 2.0);
    EXPECT_EQ(cylinderWithMaterial->height, 4.0);
    EXPECT_EQ(cylinderWithMaterial->axis.X, 1);
    EXPECT_EQ(cylinderWithMaterial->axis.Y, 0);
    EXPECT_EQ(cylinderWithMaterial->axis.Z, 0);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->color.X, 1.0);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->color.Y, 0.0);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->reflectivity, 0.8);
    EXPECT_EQ(cylinderWithMaterial->getMaterial()->transparency, 0.5);
}

TEST_F(CylinderTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    cylinderDefault->translate(translation);
    
    EXPECT_EQ(cylinderDefault->center.X, 1);
    EXPECT_EQ(cylinderDefault->center.Y, 2);
    EXPECT_EQ(cylinderDefault->center.Z, 3);
}

TEST_F(CylinderTest, RotationTest) {
    cylinderDefault->rotateX(90);
    
    EXPECT_NEAR(cylinderDefault->axis.X, 0.0, 0.001);
    EXPECT_NEAR(cylinderDefault->axis.Y, 0.0, 0.001);
    EXPECT_NEAR(cylinderDefault->axis.Z, 1.0, 0.001);
}

TEST_F(CylinderTest, HitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{-5, 0, 0}), 
                         Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfo = cylinderDefault->hit(rayHit, 0, 10);
    
    EXPECT_TRUE(hitInfo.has_value());
    if (hitInfo) {
        EXPECT_GT(hitInfo->distance, 0);
        EXPECT_NEAR(hitInfo->normal.length(), 1.0, 0.001);
    }

    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, 5, 0}), 
                          Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoMiss = cylinderDefault->hit(rayMiss, 0, 10);
    
    EXPECT_FALSE(hitInfoMiss.has_value());
}

TEST_F(CylinderTest, CloneTest) {
    auto clonedCylinder = std::dynamic_pointer_cast<RayTracer::Cylinder>(cylinderWithMaterial->clone());
    
    EXPECT_NE(clonedCylinder, nullptr);
    if (clonedCylinder) {
        EXPECT_EQ(clonedCylinder->center.X, cylinderWithMaterial->center.X);
        EXPECT_EQ(clonedCylinder->center.Y, cylinderWithMaterial->center.Y);
        EXPECT_EQ(clonedCylinder->center.Z, cylinderWithMaterial->center.Z);
        EXPECT_EQ(clonedCylinder->radius, cylinderWithMaterial->radius);
        EXPECT_EQ(clonedCylinder->height, cylinderWithMaterial->height);
        EXPECT_EQ(clonedCylinder->axis.X, cylinderWithMaterial->axis.X);
        EXPECT_EQ(clonedCylinder->axis.Y, cylinderWithMaterial->axis.Y);
        EXPECT_EQ(clonedCylinder->axis.Z, cylinderWithMaterial->axis.Z);
        EXPECT_EQ(clonedCylinder->getMaterial()->color.X, cylinderWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedCylinder->getMaterial()->color.Y, cylinderWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedCylinder->getMaterial()->color.Z, cylinderWithMaterial->getMaterial()->color.Z);
    }
}

TEST_F(CylinderTest, TypeNameTest) {
    EXPECT_EQ(cylinderDefault->getTypeName(), "cylinders");
    EXPECT_EQ(RayTracer::Cylinder::getTypeNameStatic(), "cylinders");
}

TEST_F(CylinderTest, EdgeCaseTest) {
    RayTracer::Ray rayAlmostParallel(Math::Point3D(Math::Coords{0.1, -5, 0}), 
                              Math::Vector3D(Math::Coords{0.01, 1, 0}).normalize());
    auto hitInfoParallel = cylinderDefault->hit(rayAlmostParallel, 0, 10);
    
    if (hitInfoParallel) {
        EXPECT_GT(hitInfoParallel->distance, 0);
        EXPECT_NEAR(hitInfoParallel->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayThroughBase(Math::Point3D(Math::Coords{0.1, -5, 0}), 
                                  Math::Vector3D(Math::Coords{0.01, 1, 0}).normalize());
    auto hitInfoThroughBase = cylinderDefault->hit(rayThroughBase, 0, 10);
    if (hitInfoThroughBase) {
        EXPECT_GT(hitInfoThroughBase->distance, 0);
        EXPECT_NEAR(hitInfoThroughBase->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayEdge(Math::Point3D(Math::Coords{-5, 0, 1.0}), 
                          Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoEdge = cylinderDefault->hit(rayEdge, 0, 10);
    if (hitInfoEdge) {
        EXPECT_GT(hitInfoEdge->distance, 0);
        EXPECT_NEAR(hitInfoEdge->normal.length(), 1.0, 0.001);
    }
    
    RayTracer::Ray rayAlmostHit(Math::Point3D(Math::Coords{-5, 0, 1.01}), 
                               Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoAlmostHit = cylinderDefault->hit(rayAlmostHit, 0, 10);
    
    if (hitInfoAlmostHit) {
        EXPECT_GT(hitInfoAlmostHit->distance, 0);
        EXPECT_NEAR(hitInfoAlmostHit->normal.length(), 1.0, 0.001);
    }
}

TEST_F(CylinderTest, BoundsTest) {
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitTooSmall = cylinderDefault->hit(ray, 0, 3);
    EXPECT_FALSE(hitTooSmall.has_value());
    
    auto hitNormal = cylinderDefault->hit(ray, 0, 20);
    if (hitNormal.has_value()) {
        double actualDistance = hitNormal->distance;
        auto hitAfterMin = cylinderDefault->hit(ray, actualDistance + 0.1, 20);
        
        if (hitAfterMin.has_value()) {
            EXPECT_GT(hitAfterMin->distance, actualDistance);
        }
    }
}

TEST_F(CylinderTest, RotatedHitTest) {
    auto rotatedCylinder = std::make_shared<RayTracer::Cylinder>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        1.0, 2.0);
    rotatedCylinder->rotateX(45);
    rotatedCylinder->rotateY(30);
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfo = rotatedCylinder->hit(ray, 0, 10);
    
    if (hitInfo.has_value()) {
        EXPECT_GT(hitInfo->distance, 0);
        EXPECT_NEAR(hitInfo->normal.length(), 1.0, 0.001);
    }
}

TEST_F(CylinderTest, DifferentSizeTest) {
    auto smallCylinder = std::make_shared<RayTracer::Cylinder>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        0.5, 1.0);
    
    auto largeCylinder = std::make_shared<RayTracer::Cylinder>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        2.0, 4.0);
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitSmall = smallCylinder->hit(ray, 0, 10);
    auto hitLarge = largeCylinder->hit(ray, 0, 10);
    
    EXPECT_TRUE(hitSmall.has_value());
    EXPECT_TRUE(hitLarge.has_value());
    
    if (hitSmall.has_value() && hitLarge.has_value()) {
        EXPECT_LT(hitLarge->distance, hitSmall->distance);
    }
}

} // namespace RayTracerTest