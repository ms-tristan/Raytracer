/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Tests for MobiusStrip utilities
*/

#include <gtest/gtest.h>
#include "Primitive/MobiusStrip/Utils/MobiusStripUtils.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Ray/Ray.hpp"
#include <cmath>

namespace RayTracerTest {

class MobiusStripUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        center = Math::Point3D(Math::Coords{0.0, 0.0, 0.0});
        majorRadius = 4.0;
        minorRadius = 1.0;
        thickness = 0.1;
    }

    Math::Point3D center;
    double majorRadius;
    double minorRadius;
    double thickness;
};

TEST_F(MobiusStripUtilsTest, CalculatePoint) {
    Math::Point3D point1 = RayTracer::MobiusStripUtils::calculatePoint(
        center, 0.0, 0.0, majorRadius);
    Math::Point3D point2 = RayTracer::MobiusStripUtils::calculatePoint(
        center, M_PI, 0.0, majorRadius);

    EXPECT_NEAR(point1.X, majorRadius, 1e-6);
    EXPECT_NEAR(point1.Y, 0.0, 1e-6);
    EXPECT_NEAR(point1.Z, 0.0, 1e-6);

    EXPECT_NEAR(point2.X, -majorRadius, 1e-6);
    EXPECT_NEAR(point2.Y, 0.0, 1e-6);
    EXPECT_NEAR(point2.Z, 0.0, 1e-6);

    Math::Point3D point3 = RayTracer::MobiusStripUtils::calculatePoint(
        center, 0.0, minorRadius, majorRadius);
    
    EXPECT_NEAR(point3.X, majorRadius + minorRadius, 1e-6);
    EXPECT_NEAR(point3.Y, 0.0, 1e-6);
    EXPECT_NEAR(point3.Z, 0.0, 1e-6);
}

TEST_F(MobiusStripUtilsTest, CheckBoundingSphereIntersection) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{-10, 0, 0}), 
                          Math::Vector3D(Math::Coords{1, 0, 0}));
    
    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-10, 10, 0}), 
                           Math::Vector3D(Math::Coords{0, 1, 0}));
    
    double boundingSphereRadius = majorRadius + minorRadius + thickness/2.0;
    RayTracer::Ray rayNearTangent(Math::Point3D(Math::Coords{-10, boundingSphereRadius - 0.001, 0}), 
                                 Math::Vector3D(Math::Coords{1, 0, 0}));
    
    EXPECT_TRUE(RayTracer::MobiusStripUtils::checkBoundingSphereIntersection(rayHit, center, boundingSphereRadius));
    EXPECT_FALSE(RayTracer::MobiusStripUtils::checkBoundingSphereIntersection(rayMiss, center, boundingSphereRadius));
    EXPECT_TRUE(RayTracer::MobiusStripUtils::checkBoundingSphereIntersection(rayNearTangent, center, boundingSphereRadius));
}

TEST_F(MobiusStripUtilsTest, TransformRayForRotation) {
    RayTracer::Ray originalRay(Math::Point3D(Math::Coords{1.0, 0.0, 0.0}),
                              Math::Vector3D(Math::Coords{0.0, 1.0, 0.0}));
    
    RayTracer::Ray transformedRay1 = RayTracer::MobiusStripUtils::transformRayForRotation(
        originalRay, 0.0, 0.0, 0.0);
    EXPECT_NEAR(transformedRay1.origin.X, originalRay.origin.X, 1e-6);
    EXPECT_NEAR(transformedRay1.origin.Y, originalRay.origin.Y, 1e-6);
    EXPECT_NEAR(transformedRay1.origin.Z, originalRay.origin.Z, 1e-6);
    EXPECT_NEAR(transformedRay1.direction.X, originalRay.direction.X, 1e-6);
    EXPECT_NEAR(transformedRay1.direction.Y, originalRay.direction.Y, 1e-6);
    EXPECT_NEAR(transformedRay1.direction.Z, originalRay.direction.Z, 1e-6);
    
    RayTracer::Ray transformedRay2 = RayTracer::MobiusStripUtils::transformRayForRotation(
        originalRay, 0.0, 0.0, 90.0);
    EXPECT_NEAR(transformedRay2.origin.X, 0.0, 1e-6);
    EXPECT_NEAR(transformedRay2.origin.Y, -1.0, 1e-6);
    EXPECT_NEAR(transformedRay2.origin.Z, 0.0, 1e-6);
    EXPECT_NEAR(transformedRay2.direction.X, 1.0, 1e-6);
    EXPECT_NEAR(transformedRay2.direction.Y, 0.0, 1e-6);
    EXPECT_NEAR(transformedRay2.direction.Z, 0.0, 1e-6);
}

TEST_F(MobiusStripUtilsTest, RotateNormal) {
    Math::Vector3D originalNormal(Math::Coords{1.0, 0.0, 0.0});
    
    Math::Vector3D rotatedNormal1 = RayTracer::MobiusStripUtils::rotateNormal(
        originalNormal, 0.0, 0.0, 0.0);
    EXPECT_NEAR(rotatedNormal1.X, originalNormal.X, 1e-6);
    EXPECT_NEAR(rotatedNormal1.Y, originalNormal.Y, 1e-6);
    EXPECT_NEAR(rotatedNormal1.Z, originalNormal.Z, 1e-6);
    
    Math::Vector3D rotatedNormal2 = RayTracer::MobiusStripUtils::rotateNormal(
        originalNormal, 0.0, 0.0, 90.0);
    EXPECT_NEAR(rotatedNormal2.X, 0.0, 1e-6);
    EXPECT_NEAR(rotatedNormal2.Y, 1.0, 1e-6);
    EXPECT_NEAR(rotatedNormal2.Z, 0.0, 1e-6);
    
    Math::Vector3D rotatedNormal3 = RayTracer::MobiusStripUtils::rotateNormal(
        originalNormal, 90.0, 90.0, 0.0);
    EXPECT_NEAR(rotatedNormal3.X, 0.0, 1e-6);
    EXPECT_NEAR(rotatedNormal3.Y, 0.0, 1e-6);
    EXPECT_NEAR(rotatedNormal3.Z, -1.0, 1e-6);
}

}  // namespace RayTracerTest