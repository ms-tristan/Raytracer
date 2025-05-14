// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for TangleCube primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include <cmath>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/TangleCube/TangleCube.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class TangleCubeTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D center(Math::Coords{0, 0, 0});
        Math::Vector3D axis(Math::Coords{0, 1, 0});
        cubeDefault = std::make_shared<RayTracer::TangleCube>(center, axis, 1.0, 0.2);

        Math::Point3D center2(Math::Coords{2, 3, 4});
        Math::Vector3D axis2(Math::Coords{1, 0, 0});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 0.0, 0.0});
        material->reflectivity = 0.8;
        material->transparency = 0.5;
        cubeWithMaterial = std::make_shared<RayTracer::TangleCube>(center2, axis2, 2.0, 0.4, material);
    }

    std::shared_ptr<RayTracer::TangleCube> cubeDefault;
    std::shared_ptr<RayTracer::TangleCube> cubeWithMaterial;
};

TEST_F(TangleCubeTest, CreationTest) {
    EXPECT_EQ(cubeDefault->center.X, 0);
    EXPECT_EQ(cubeDefault->center.Y, 0);
    EXPECT_EQ(cubeDefault->center.Z, 0);
    EXPECT_EQ(cubeDefault->size, 1.0);
    EXPECT_EQ(cubeDefault->spacing, 0.2);

    EXPECT_EQ(cubeWithMaterial->center.X, 2);
    EXPECT_EQ(cubeWithMaterial->center.Y, 3);
    EXPECT_EQ(cubeWithMaterial->center.Z, 4);
    EXPECT_EQ(cubeWithMaterial->size, 2.0);
    EXPECT_EQ(cubeWithMaterial->spacing, 0.4);
    EXPECT_EQ(cubeWithMaterial->getMaterial()->color.X, 1.0);
    EXPECT_EQ(cubeWithMaterial->getMaterial()->color.Y, 0.0);
    EXPECT_EQ(cubeWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(cubeWithMaterial->getMaterial()->reflectivity, 0.8);
    EXPECT_EQ(cubeWithMaterial->getMaterial()->transparency, 0.5);
}

TEST_F(TangleCubeTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    cubeDefault->translate(translation);
    
    EXPECT_EQ(cubeDefault->center.X, 1);
    EXPECT_EQ(cubeDefault->center.Y, 2);
    EXPECT_EQ(cubeDefault->center.Z, 3);
    
    EXPECT_EQ(cubeDefault->size, 1.0);
    EXPECT_EQ(cubeDefault->spacing, 0.2);
}

TEST_F(TangleCubeTest, RotationTest) {
    Math::Point3D initialCenter = cubeDefault->center;
    
    cubeDefault->rotateX(45);
    cubeDefault->rotateY(30);
    cubeDefault->rotateZ(60);
    
    EXPECT_EQ(cubeDefault->center.X, initialCenter.X);
    EXPECT_EQ(cubeDefault->center.Y, initialCenter.Y);
    EXPECT_EQ(cubeDefault->center.Z, initialCenter.Z);
    
    EXPECT_EQ(cubeDefault->size, 1.0);
    EXPECT_EQ(cubeDefault->spacing, 0.2);
}

TEST_F(TangleCubeTest, CloneTest) {
    auto clonedCube = std::dynamic_pointer_cast<RayTracer::TangleCube>(cubeWithMaterial->clone());
    
    EXPECT_NE(clonedCube, nullptr);
    if (clonedCube) {
        EXPECT_EQ(clonedCube->center.X, cubeWithMaterial->center.X);
        EXPECT_EQ(clonedCube->center.Y, cubeWithMaterial->center.Y);
        EXPECT_EQ(clonedCube->center.Z, cubeWithMaterial->center.Z);
        EXPECT_EQ(clonedCube->size, cubeWithMaterial->size);
        EXPECT_EQ(clonedCube->spacing, cubeWithMaterial->spacing);
        EXPECT_EQ(clonedCube->getMaterial()->color.X, cubeWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedCube->getMaterial()->color.Y, cubeWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedCube->getMaterial()->color.Z, cubeWithMaterial->getMaterial()->color.Z);
        EXPECT_EQ(clonedCube->getMaterial()->reflectivity, cubeWithMaterial->getMaterial()->reflectivity);
        EXPECT_EQ(clonedCube->getMaterial()->transparency, cubeWithMaterial->getMaterial()->transparency);
    }
}

TEST_F(TangleCubeTest, TypeNameTest) {
    EXPECT_EQ(cubeDefault->getTypeName(), "tanglecubes");
    EXPECT_EQ(RayTracer::TangleCube::getTypeNameStatic(), "tanglecubes");
}

TEST_F(TangleCubeTest, HitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}));
    auto hitInfo = cubeDefault->hit(rayHit, 0, 20);
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-20, -20, -20}), Math::Vector3D(Math::Coords{0, 1, 0}));
    auto hitInfoMiss = cubeDefault->hit(rayMiss, 0, 10);
    
    if (hitInfo.has_value()) {
        EXPECT_GT(hitInfo->distance, 0);
        EXPECT_NEAR(hitInfo->normal.length(), 1.0, 0.001);
    }
}

TEST_F(TangleCubeTest, BoundsTest) {
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}));
    
    auto hitInfoShortRange = cubeDefault->hit(ray, 0, 2);
    EXPECT_FALSE(hitInfoShortRange.has_value());
    
    auto hitInfoLongRange = cubeDefault->hit(ray, 0, 10);
    if (hitInfoLongRange.has_value()) {
        EXPECT_GT(hitInfoLongRange->distance, 0);
        EXPECT_LE(hitInfoLongRange->distance, 10);
    }
}

TEST_F(TangleCubeTest, ScaleTest) {
    auto smallCube = std::make_shared<RayTracer::TangleCube>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        0.5,
        0.2);
    
    auto largeCube = std::make_shared<RayTracer::TangleCube>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        3.0,
        0.2);
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), 
                     Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitSmall = smallCube->hit(ray, 0, 10);
    auto hitLarge = largeCube->hit(ray, 0, 10);
    
    if (hitSmall.has_value() && hitLarge.has_value()) {
        EXPECT_LT(hitLarge->distance, hitSmall->distance);
    }
    else {
        if (hitSmall.has_value()) {
            EXPECT_GT(hitSmall->distance, 0);
            EXPECT_NEAR(hitSmall->normal.length(), 1.0, 0.001);
        }
        if (hitLarge.has_value()) {
            EXPECT_GT(hitLarge->distance, 0);
            EXPECT_NEAR(hitLarge->normal.length(), 1.0, 0.001);
        }
    }
}

TEST_F(TangleCubeTest, TransformedCubeTest) {
    auto transformedCube = std::make_shared<RayTracer::TangleCube>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}),
        1.0, 0.2);
    
    transformedCube->rotateX(45);
    transformedCube->rotateZ(30);
    transformedCube->translate(Math::Vector3D(Math::Coords{2, 3, 4}));
    
    RayTracer::Ray rayToTransformed(Math::Point3D(Math::Coords{-5, 3, 4}), 
                                  Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitTransformed = transformedCube->hit(rayToTransformed, 0, 20);
    
    if (hitTransformed.has_value()) {
        EXPECT_GT(hitTransformed->distance, 0);
        EXPECT_NEAR(hitTransformed->normal.length(), 1.0, 0.001);
    }
    
    EXPECT_EQ(transformedCube->center.X, 2);
    EXPECT_EQ(transformedCube->center.Y, 3);
    EXPECT_EQ(transformedCube->center.Z, 4);
}

TEST_F(TangleCubeTest, MultipleRaysTest) {
    RayTracer::Ray rayFromPosX(Math::Point3D(Math::Coords{5, 0, 0}), 
                             Math::Vector3D(Math::Coords{-1, 0, 0}).normalize());
    auto hitPosX = cubeDefault->hit(rayFromPosX, 0, 10);
    
    RayTracer::Ray rayFromNegX(Math::Point3D(Math::Coords{-5, 0, 0}), 
                             Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitNegX = cubeDefault->hit(rayFromNegX, 0, 10);
    
    RayTracer::Ray rayFromPosY(Math::Point3D(Math::Coords{0, 5, 0}), 
                             Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitPosY = cubeDefault->hit(rayFromPosY, 0, 10);
    
    RayTracer::Ray rayFromNegY(Math::Point3D(Math::Coords{0, -5, 0}), 
                             Math::Vector3D(Math::Coords{0, 1, 0}).normalize());
    auto hitNegY = cubeDefault->hit(rayFromNegY, 0, 10);
    
    RayTracer::Ray rayFromPosZ(Math::Point3D(Math::Coords{0, 0, 5}), 
                             Math::Vector3D(Math::Coords{0, 0, -1}).normalize());
    auto hitPosZ = cubeDefault->hit(rayFromPosZ, 0, 10);
    
    RayTracer::Ray rayFromNegZ(Math::Point3D(Math::Coords{0, 0, -5}), 
                             Math::Vector3D(Math::Coords{0, 0, 1}).normalize());
    auto hitNegZ = cubeDefault->hit(rayFromNegZ, 0, 10);
    
    if (hitPosX.has_value()) {
        EXPECT_GT(hitPosX->distance, 0);
        EXPECT_NEAR(hitPosX->normal.length(), 1.0, 0.001);
    }
    if (hitNegX.has_value()) {
        EXPECT_GT(hitNegX->distance, 0);
        EXPECT_NEAR(hitNegX->normal.length(), 1.0, 0.001);
    }
    if (hitPosY.has_value()) {
        EXPECT_GT(hitPosY->distance, 0);
        EXPECT_NEAR(hitPosY->normal.length(), 1.0, 0.001);
    }
    if (hitNegY.has_value()) {
        EXPECT_GT(hitNegY->distance, 0);
        EXPECT_NEAR(hitNegY->normal.length(), 1.0, 0.001);
    }
    if (hitPosZ.has_value()) {
        EXPECT_GT(hitPosZ->distance, 0);
        EXPECT_NEAR(hitPosZ->normal.length(), 1.0, 0.001);
    }
    if (hitNegZ.has_value()) {
        EXPECT_GT(hitNegZ->distance, 0);
        EXPECT_NEAR(hitNegZ->normal.length(), 1.0, 0.001);
    }
}

} // namespace RayTracerTest