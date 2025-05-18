/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for Sphere primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "../src/Math/Point3D/Point3D.hpp"
#include "../src/Math/Vector3D/Vector3D.hpp"
#include "../src/Primitive/Sphere/Sphere.hpp"
#include "../src/Ray/Ray.hpp"

namespace RayTracerTest {

class SphereTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D center(Math::Coords{0, 0, 0});
        sphereDefault = std::make_shared<RayTracer::Sphere>(center, 1.0);

        Math::Point3D center2(Math::Coords{2, 3, 4});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 0.0, 0.0}); 
        material->reflectivity = 0.8;
        material->transparency = 0.5;
        sphereWithMaterial = std::make_shared<RayTracer::Sphere>(center2, 2.0, material);
    }

    std::shared_ptr<RayTracer::Sphere> sphereDefault;
    std::shared_ptr<RayTracer::Sphere> sphereWithMaterial;
};

TEST_F(SphereTest, CreationTest) {
    EXPECT_EQ(sphereDefault->center.X, 0);
    EXPECT_EQ(sphereDefault->center.Y, 0);
    EXPECT_EQ(sphereDefault->center.Z, 0);
    EXPECT_EQ(sphereDefault->radius, 1.0);

    EXPECT_EQ(sphereWithMaterial->center.X, 2);
    EXPECT_EQ(sphereWithMaterial->center.Y, 3);
    EXPECT_EQ(sphereWithMaterial->center.Z, 4);
    EXPECT_EQ(sphereWithMaterial->radius, 2.0);
    EXPECT_EQ(sphereWithMaterial->getMaterial()->color.X, 1.0);
    EXPECT_EQ(sphereWithMaterial->getMaterial()->color.Y, 0.0);
    EXPECT_EQ(sphereWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(sphereWithMaterial->getMaterial()->reflectivity, 0.8);
    EXPECT_EQ(sphereWithMaterial->getMaterial()->transparency, 0.5);
}

TEST_F(SphereTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    sphereDefault->translate(translation);
    
    EXPECT_EQ(sphereDefault->center.X, 1);
    EXPECT_EQ(sphereDefault->center.Y, 2);
    EXPECT_EQ(sphereDefault->center.Z, 3);
}

TEST_F(SphereTest, RotationTest) {
    sphereDefault->rotateX(45);
    sphereDefault->rotateY(30);
    sphereDefault->rotateZ(60);
    
    EXPECT_EQ(sphereDefault->center.X, 0);
    EXPECT_EQ(sphereDefault->center.Y, 0);
    EXPECT_EQ(sphereDefault->center.Z, 0);
}

TEST_F(SphereTest, HitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}));
    auto hitInfo = sphereDefault->hit(rayHit, 0, 10);
    
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

    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, 2, 0}), Math::Vector3D(Math::Coords{1, 0, 0}));
    auto hitInfoMiss = sphereDefault->hit(rayMiss, 0, 10);
    
    EXPECT_FALSE(hitInfoMiss.has_value());
}

TEST_F(SphereTest, CloneTest) {
    auto clonedSphere = std::dynamic_pointer_cast<RayTracer::Sphere>(sphereWithMaterial->clone());
    
    EXPECT_NE(clonedSphere, nullptr);
    if (clonedSphere) {
        EXPECT_EQ(clonedSphere->center.X, sphereWithMaterial->center.X);
        EXPECT_EQ(clonedSphere->center.Y, sphereWithMaterial->center.Y);
        EXPECT_EQ(clonedSphere->center.Z, sphereWithMaterial->center.Z);
        EXPECT_EQ(clonedSphere->radius, sphereWithMaterial->radius);
        EXPECT_EQ(clonedSphere->getMaterial()->color.X, sphereWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedSphere->getMaterial()->color.Y, sphereWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedSphere->getMaterial()->color.Z, sphereWithMaterial->getMaterial()->color.Z);
        EXPECT_EQ(clonedSphere->getMaterial()->reflectivity, sphereWithMaterial->getMaterial()->reflectivity);
        EXPECT_EQ(clonedSphere->getMaterial()->transparency, sphereWithMaterial->getMaterial()->transparency);
    }
}

TEST_F(SphereTest, TypeNameTest) {
    EXPECT_EQ(sphereDefault->getTypeName(), "spheres");
    EXPECT_EQ(RayTracer::Sphere::getTypeNameStatic(), "spheres");
}

TEST_F(SphereTest, ComprehensiveHitTest) {
    RayTracer::Ray rayCenterHit(Math::Point3D(Math::Coords{-5, 0, 0}), 
                              Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitCenterInfo = sphereDefault->hit(rayCenterHit, 0, 10);
    
    EXPECT_TRUE(hitCenterInfo.has_value());
    if (hitCenterInfo) {
        EXPECT_NEAR(hitCenterInfo->hitPoint.X, -1.0, 0.001);
        EXPECT_NEAR(hitCenterInfo->normal.X, -1.0, 0.001);
        EXPECT_NEAR(hitCenterInfo->normal.Y, 0.0, 0.001);
        EXPECT_NEAR(hitCenterInfo->normal.Z, 0.0, 0.001);
    }
    
    RayTracer::Ray rayFarHit(Math::Point3D(Math::Coords{5, 0, 0}), 
                           Math::Vector3D(Math::Coords{-1, 0, 0}).normalize());
    auto hitFarInfo = sphereDefault->hit(rayFarHit, 0, 10);
    
    EXPECT_TRUE(hitFarInfo.has_value());
    if (hitFarInfo) {
        EXPECT_NEAR(hitFarInfo->hitPoint.X, 1.0, 0.001);
        EXPECT_NEAR(hitFarInfo->normal.X, 1.0, 0.001);
    }
    
    RayTracer::Ray rayTangent(Math::Point3D(Math::Coords{-5, 1, 0}), 
                            Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitTangentInfo = sphereDefault->hit(rayTangent, 0, 10);
    
    if (hitTangentInfo) {
        EXPECT_GT(hitTangentInfo->distance, 0);
        EXPECT_NEAR(hitTangentInfo->normal.length(), 1.0, 0.001);
    }
}

TEST_F(SphereTest, BoundsTest) {
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitTooSmall = sphereDefault->hit(ray, 0, 3);
    EXPECT_FALSE(hitTooSmall.has_value());
    
    auto hitNormal = sphereDefault->hit(ray, 0, 10);
    if (hitNormal.has_value()) {
        double actualDistance = hitNormal->distance;
        auto hitAfterMin = sphereDefault->hit(ray, actualDistance + 0.1, 10);
        
        if (hitAfterMin.has_value()) {
            EXPECT_GT(hitAfterMin->distance, actualDistance);
        }
    }
}

TEST_F(SphereTest, TransformedSphereTest) {
    auto translatedSphere = std::make_shared<RayTracer::Sphere>(
        Math::Point3D(Math::Coords{0, 0, 0}), 1.0);
    translatedSphere->translate(Math::Vector3D(Math::Coords{3, 0, 0}));
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitTransInfo = translatedSphere->hit(ray, 0, 20);
    
    EXPECT_TRUE(hitTransInfo.has_value());
    if (hitTransInfo) {
        EXPECT_NEAR(hitTransInfo->hitPoint.X, 2.0, 0.001);
        EXPECT_NEAR(hitTransInfo->normal.X, -1.0, 0.001);
    }
    
    auto rotatedSphere = std::make_shared<RayTracer::Sphere>(
        Math::Point3D(Math::Coords{0, 0, 0}), 1.0);
    rotatedSphere->rotateX(45);
    rotatedSphere->rotateY(30);
    
    auto hitRotInfo = rotatedSphere->hit(ray, 0, 10);
    auto sphereDefaultHit = sphereDefault->hit(ray, 0, 10);
    
    EXPECT_TRUE(hitRotInfo.has_value());
    if (hitRotInfo && sphereDefaultHit.has_value()) {
        EXPECT_NEAR(hitRotInfo->distance, sphereDefaultHit->distance, 0.001);
    }
}

TEST_F(SphereTest, InsideOutsideTest) {
    RayTracer::Ray rayFromInside(Math::Point3D(Math::Coords{0, 0, 0}), 
                               Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitFromInsideInfo = sphereDefault->hit(rayFromInside, 0, 10);
    
    EXPECT_TRUE(hitFromInsideInfo.has_value());
    if (hitFromInsideInfo) {
        EXPECT_NEAR(hitFromInsideInfo->distance, 1.0, 0.001);
        EXPECT_NEAR(std::abs(hitFromInsideInfo->normal.X), 1.0, 0.001);
    }
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, 2, 0}), 
                         Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitMissInfo = sphereDefault->hit(rayMiss, 0, 10);
    
    EXPECT_FALSE(hitMissInfo.has_value());
}

TEST_F(SphereTest, DifferentSizeTest) {
    auto smallSphere = std::make_shared<RayTracer::Sphere>(
        Math::Point3D(Math::Coords{0, 0, 0}), 0.5);
        
    auto largeSphere = std::make_shared<RayTracer::Sphere>(
        Math::Point3D(Math::Coords{0, 0, 0}), 2.0);
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitSmall = smallSphere->hit(ray, 0, 10);
    auto hitLarge = largeSphere->hit(ray, 0, 10);
    
    EXPECT_TRUE(hitSmall.has_value());
    EXPECT_TRUE(hitLarge.has_value());
    
    if (hitSmall.has_value() && hitLarge.has_value()) {
        EXPECT_LT(hitLarge->distance, hitSmall->distance);
        
        EXPECT_NEAR(hitLarge->hitPoint.X, -2.0, 0.001);
        EXPECT_NEAR(hitSmall->hitPoint.X, -0.5, 0.001);
    }
}

} // namespace RayTracerTest