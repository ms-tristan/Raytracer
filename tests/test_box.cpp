/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for Box primitive
*/

#include <gtest/gtest.h>
#include <memory>
#include "Math/Point3D/Point3D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Primitive/Box/Box.hpp"
#include "Ray/Ray.hpp"

namespace RayTracerTest {

class BoxTest : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Point3D center(Math::Coords{0, 0, 0});
        Math::Vector3D dimensions(Math::Coords{1, 1, 1});
        boxDefault = std::make_shared<RayTracer::Box>(center, dimensions);
        Math::Point3D center2(Math::Coords{2, 3, 4});
        Math::Vector3D dimensions2(Math::Coords{2, 3, 4});
        auto material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{0.0, 1.0, 0.0});
        material->reflectivity = 0.6;
        material->transparency = 0.3;
        boxWithMaterial = std::make_shared<RayTracer::Box>(center2, dimensions2, material);
    }

    std::shared_ptr<RayTracer::Box> boxDefault;
    std::shared_ptr<RayTracer::Box> boxWithMaterial;
};

TEST_F(BoxTest, CreationTest) {
    EXPECT_EQ(boxDefault->center.X, 0);
    EXPECT_EQ(boxDefault->center.Y, 0);
    EXPECT_EQ(boxDefault->center.Z, 0);
    EXPECT_EQ(boxDefault->dimensions.X, 1);
    EXPECT_EQ(boxDefault->dimensions.Y, 1);
    EXPECT_EQ(boxDefault->dimensions.Z, 1);

    EXPECT_EQ(boxWithMaterial->center.X, 2);
    EXPECT_EQ(boxWithMaterial->center.Y, 3);
    EXPECT_EQ(boxWithMaterial->center.Z, 4);
    EXPECT_EQ(boxWithMaterial->dimensions.X, 2);
    EXPECT_EQ(boxWithMaterial->dimensions.Y, 3);
    EXPECT_EQ(boxWithMaterial->dimensions.Z, 4);
    EXPECT_EQ(boxWithMaterial->getMaterial()->color.X, 0.0);
    EXPECT_EQ(boxWithMaterial->getMaterial()->color.Y, 1.0);
    EXPECT_EQ(boxWithMaterial->getMaterial()->color.Z, 0.0);
    EXPECT_EQ(boxWithMaterial->getMaterial()->reflectivity, 0.6);
    EXPECT_EQ(boxWithMaterial->getMaterial()->transparency, 0.3);
}

TEST_F(BoxTest, TranslateTest) {
    Math::Vector3D translation(Math::Coords{1, 2, 3});
    boxDefault->translate(translation);
    
    EXPECT_EQ(boxDefault->center.X, 1);
    EXPECT_EQ(boxDefault->center.Y, 2);
    EXPECT_EQ(boxDefault->center.Z, 3);
}

TEST_F(BoxTest, RotationTest) {
    boxDefault->rotateX(45);
    boxDefault->rotateY(30);
    boxDefault->rotateZ(60);
    
    EXPECT_EQ(boxDefault->center.X, 0);
    EXPECT_EQ(boxDefault->center.Y, 0);
    EXPECT_EQ(boxDefault->center.Z, 0);
}

TEST_F(BoxTest, HitTest) {
    RayTracer::Ray rayHit(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfo = boxDefault->hit(rayHit, 0, 10);
    
    EXPECT_TRUE(hitInfo.has_value());
    if (hitInfo) {
        EXPECT_NEAR(hitInfo->distance, 4.0, 0.001);
        EXPECT_NEAR(hitInfo->normal.X, -1.0, 0.001);
        EXPECT_NEAR(hitInfo->normal.Y, 0.0, 0.001);
        EXPECT_NEAR(hitInfo->normal.Z, 0.0, 0.001);
    }

    RayTracer::Ray rayMiss(Math::Point3D(Math::Coords{-5, 2, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInfoMiss = boxDefault->hit(rayMiss, 0, 10);
    
    EXPECT_FALSE(hitInfoMiss.has_value());
}

TEST_F(BoxTest, CloneTest) {
    auto clonedBox = std::dynamic_pointer_cast<RayTracer::Box>(boxWithMaterial->clone());
    
    EXPECT_NE(clonedBox, nullptr);
    if (clonedBox) {
        EXPECT_EQ(clonedBox->center.X, boxWithMaterial->center.X);
        EXPECT_EQ(clonedBox->center.Y, boxWithMaterial->center.Y);
        EXPECT_EQ(clonedBox->center.Z, boxWithMaterial->center.Z);
        EXPECT_EQ(clonedBox->dimensions.X, boxWithMaterial->dimensions.X);
        EXPECT_EQ(clonedBox->dimensions.Y, boxWithMaterial->dimensions.Y);
        EXPECT_EQ(clonedBox->dimensions.Z, boxWithMaterial->dimensions.Z);
        EXPECT_EQ(clonedBox->getMaterial()->color.X, boxWithMaterial->getMaterial()->color.X);
        EXPECT_EQ(clonedBox->getMaterial()->color.Y, boxWithMaterial->getMaterial()->color.Y);
        EXPECT_EQ(clonedBox->getMaterial()->color.Z, boxWithMaterial->getMaterial()->color.Z);
        EXPECT_EQ(clonedBox->getMaterial()->reflectivity, boxWithMaterial->getMaterial()->reflectivity);
        EXPECT_EQ(clonedBox->getMaterial()->transparency, boxWithMaterial->getMaterial()->transparency);
    }
}

TEST_F(BoxTest, TypeNameTest) {
    EXPECT_EQ(boxDefault->getTypeName(), "boxes");
    EXPECT_EQ(RayTracer::Box::getTypeNameStatic(), "boxes");
}

TEST_F(BoxTest, ComprehensiveHitTest) {
    RayTracer::Ray rayLeft(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitLeft = boxDefault->hit(rayLeft, 0, 10);
    EXPECT_TRUE(hitLeft.has_value());
    if (hitLeft) {
        EXPECT_NEAR(hitLeft->normal.X, -1.0, 0.001);
        EXPECT_NEAR(hitLeft->normal.Y, 0.0, 0.001);
        EXPECT_NEAR(hitLeft->normal.Z, 0.0, 0.001);
    }
    
    RayTracer::Ray rayRight(Math::Point3D(Math::Coords{5, 0, 0}), Math::Vector3D(Math::Coords{-1, 0, 0}).normalize());
    auto hitRight = boxDefault->hit(rayRight, 0, 10);
    EXPECT_TRUE(hitRight.has_value());
    if (hitRight) {
        EXPECT_NEAR(hitRight->normal.X, 1.0, 0.001);
        EXPECT_NEAR(hitRight->normal.Y, 0.0, 0.001);
        EXPECT_NEAR(hitRight->normal.Z, 0.0, 0.001);
    }
    
    RayTracer::Ray rayBottom(Math::Point3D(Math::Coords{0, -5, 0}), Math::Vector3D(Math::Coords{0, 1, 0}).normalize());
    auto hitBottom = boxDefault->hit(rayBottom, 0, 10);
    EXPECT_TRUE(hitBottom.has_value());
    if (hitBottom) {
        EXPECT_NEAR(hitBottom->normal.X, 0.0, 0.001);
        EXPECT_NEAR(hitBottom->normal.Y, -1.0, 0.001);
        EXPECT_NEAR(hitBottom->normal.Z, 0.0, 0.001);
    }
    
    RayTracer::Ray rayTop(Math::Point3D(Math::Coords{0, 5, 0}), Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitTop = boxDefault->hit(rayTop, 0, 10);
    EXPECT_TRUE(hitTop.has_value());
    if (hitTop) {
        EXPECT_NEAR(hitTop->normal.X, 0.0, 0.001);
        EXPECT_NEAR(hitTop->normal.Y, 1.0, 0.001);
        EXPECT_NEAR(hitTop->normal.Z, 0.0, 0.001);
    }
    
    RayTracer::Ray rayBack(Math::Point3D(Math::Coords{0, 0, -5}), Math::Vector3D(Math::Coords{0, 0, 1}).normalize());
    auto hitBack = boxDefault->hit(rayBack, 0, 10);
    EXPECT_TRUE(hitBack.has_value());
    if (hitBack) {
        EXPECT_NEAR(hitBack->normal.X, 0.0, 0.001);
        EXPECT_NEAR(hitBack->normal.Y, 0.0, 0.001);
        EXPECT_NEAR(hitBack->normal.Z, -1.0, 0.001);
    }
    
    RayTracer::Ray rayFront(Math::Point3D(Math::Coords{0, 0, 5}), Math::Vector3D(Math::Coords{0, 0, -1}).normalize());
    auto hitFront = boxDefault->hit(rayFront, 0, 10);
    EXPECT_TRUE(hitFront.has_value());
    if (hitFront) {
        EXPECT_NEAR(hitFront->normal.X, 0.0, 0.001);
        EXPECT_NEAR(hitFront->normal.Y, 0.0, 0.001);
        EXPECT_NEAR(hitFront->normal.Z, 1.0, 0.001);
    }
    
    RayTracer::Ray rayDiagonal(
        Math::Point3D(Math::Coords{-5, -5, -5}),
        Math::Vector3D(Math::Coords{1, 1, 1}).normalize());
    auto hitDiagonal = boxDefault->hit(rayDiagonal, 0, 20);
    EXPECT_TRUE(hitDiagonal.has_value());
    
    RayTracer::Ray rayMissCorner(
        Math::Point3D(Math::Coords{-5, 1.5, 1.5}), 
        Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitMissCorner = boxDefault->hit(rayMissCorner, 0, 10);
    EXPECT_FALSE(hitMissCorner.has_value());
}

TEST_F(BoxTest, InsideOutsideTest) {

    RayTracer::Ray rayInsideRight(Math::Point3D(Math::Coords{0, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitInsideRight = boxDefault->hit(rayInsideRight, 0, 10);
    EXPECT_TRUE(hitInsideRight.has_value());
    if (hitInsideRight) {
        EXPECT_NEAR(hitInsideRight->distance, 1.0, 0.001);
        EXPECT_NEAR(hitInsideRight->normal.X, 1.0, 0.001);
    }
    
    RayTracer::Ray rayNearSurface(Math::Point3D(Math::Coords{0.499, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitNearSurface = boxDefault->hit(rayNearSurface, 0, 10);
    EXPECT_TRUE(hitNearSurface.has_value());
    if (hitNearSurface) {
        EXPECT_NEAR(hitNearSurface->distance, 0.501, 0.001);
    }
}

TEST_F(BoxTest, BoundsTest) {
    
    RayTracer::Ray ray(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    
    auto hitTooSmall = boxDefault->hit(ray, 0, 3.9);
    EXPECT_FALSE(hitTooSmall.has_value());

    auto hitAtMax = boxDefault->hit(ray, 0, 4.5);
    EXPECT_TRUE(hitAtMax.has_value());
    
    auto hitAfterMin = boxDefault->hit(ray, 4.5, 10);
    EXPECT_FALSE(hitAfterMin.has_value());
    
    auto hitBeforeMin = boxDefault->hit(ray, 3.9, 10);
    EXPECT_TRUE(hitBeforeMin.has_value());
}

TEST_F(BoxTest, RotatedHitTest) {
    
    auto rotatedBox = std::make_shared<RayTracer::Box>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{1, 1, 1}));
    rotatedBox->rotateY(45);
    
    RayTracer::Ray rayToRotated(Math::Point3D(Math::Coords{-5, 0, -5}), Math::Vector3D(Math::Coords{1, 0, 1}).normalize());
    auto hitRotated = rotatedBox->hit(rayToRotated, 0, 20);
    EXPECT_TRUE(hitRotated.has_value());
    
    RayTracer::Ray rayMissRotated(Math::Point3D(Math::Coords{-5, 0, -2}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitMissRotated = rotatedBox->hit(rayMissRotated, 0, 3);
    EXPECT_FALSE(hitMissRotated.has_value());
    
    auto complexRotatedBox = std::make_shared<RayTracer::Box>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{1, 1, 1}));
    complexRotatedBox->rotateX(30);
    complexRotatedBox->rotateY(45);
    complexRotatedBox->rotateZ(60);
    
    RayTracer::Ray rayToComplex(Math::Point3D(Math::Coords{-5, -5, -5}), Math::Vector3D(Math::Coords{1, 1, 1}).normalize());
    auto hitComplex = complexRotatedBox->hit(rayToComplex, 0, 20);
    if (hitComplex.has_value()) {
        EXPECT_GT(hitComplex->distance, 0);
        EXPECT_NEAR(hitComplex->normal.length(), 1.0, 0.001);
    }
}

TEST_F(BoxTest, NonUniformScaleTest) {
    
    auto longBox = std::make_shared<RayTracer::Box>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{4, 1, 1}));
    
    RayTracer::Ray rayToLongEnd(Math::Point3D(Math::Coords{-5, 0, 0}), Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitLongEnd = longBox->hit(rayToLongEnd, 0, 10);
    EXPECT_TRUE(hitLongEnd.has_value());
    if (hitLongEnd) {
        EXPECT_NEAR(hitLongEnd->distance, 1, 0.001);
    }
    
    auto flatBox = std::make_shared<RayTracer::Box>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{3, 0.1, 3}));
    
    RayTracer::Ray rayDown(Math::Point3D(Math::Coords{0, 5, 0}), Math::Vector3D(Math::Coords{0, -1, 0}).normalize());
    auto hitFlat = flatBox->hit(rayDown, 0, 10);
    EXPECT_TRUE(hitFlat.has_value());
    if (hitFlat) {
        EXPECT_NEAR(hitFlat->distance, 4.9, 0.001);
        EXPECT_NEAR(hitFlat->normal.Y, 1.0, 0.001);
    }
}

TEST_F(BoxTest, RayParallelToSurfaceTest) {
    
    RayTracer::Ray rayParallelMiss(
        Math::Point3D(Math::Coords{0, 2, 0}),
        Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitParallelMiss = boxDefault->hit(rayParallelMiss, 0, 10);
    EXPECT_FALSE(hitParallelMiss.has_value());
    
    RayTracer::Ray rayParallelEdge(
        Math::Point3D(Math::Coords{-5, 0.5, 0.5}),
        Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitParallelEdge = boxDefault->hit(rayParallelEdge, 0, 10);
    EXPECT_TRUE(hitParallelEdge.has_value());
    
    RayTracer::Ray rayParallelThrough(
        Math::Point3D(Math::Coords{-5, 0, 0.25}),
        Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitParallelThrough = boxDefault->hit(rayParallelThrough, 0, 10);
    EXPECT_TRUE(hitParallelThrough.has_value());
}

TEST_F(BoxTest, EdgeCasesHitTest) {

    RayTracer::Ray rayEdgeHit(
        Math::Point3D(Math::Coords{-5, 0.499, 0.499}),
        Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitEdge = boxDefault->hit(rayEdgeHit, 0, 10);
    EXPECT_TRUE(hitEdge.has_value());

    RayTracer::Ray rayEdgeMiss(
        Math::Point3D(Math::Coords{-5, 1.5, 1.5}),
        Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto missEdge = boxDefault->hit(rayEdgeMiss, 0, 10);
    EXPECT_FALSE(missEdge.has_value());

    RayTracer::Ray rayZeroLength(
        Math::Point3D(Math::Coords{0.5, 0, 0}),
        Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto zeroHit = boxDefault->hit(rayZeroLength, 0.001, 10);

    RayTracer::Ray rayNormal(Math::Point3D(Math::Coords{-5, 0, 0}), 
                           Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hitLargeTMax = boxDefault->hit(rayNormal, 0, 1e6);
    EXPECT_TRUE(hitLargeTMax.has_value());

    auto hitSmallTMinTMax = boxDefault->hit(rayNormal, 4.5, 4.51);
    EXPECT_FALSE(hitSmallTMinTMax.has_value());

    RayTracer::Ray rayNonNormalized(
        Math::Point3D(Math::Coords{-5, 0, 0}),
        Math::Vector3D(Math::Coords{2, 0, 0}));
    auto hitNonNormalized = boxDefault->hit(rayNonNormalized, 0, 10);
    EXPECT_TRUE(hitNonNormalized.has_value());
}

TEST_F(BoxTest, TransformedBoxTests) {
    auto transformedBox = std::make_shared<RayTracer::Box>(
        Math::Point3D(Math::Coords{0, 0, 0}),
        Math::Vector3D(Math::Coords{1, 1, 1}));
    
    transformedBox->translate(Math::Vector3D(Math::Coords{2, 3, 4}));
    transformedBox->rotateX(30);
    transformedBox->rotateY(45);
    transformedBox->rotateZ(60);
    
    RayTracer::Ray ray1(
        Math::Point3D(Math::Coords{-5, 3, 4}),
        Math::Vector3D(Math::Coords{1, 0, 0}).normalize());
    auto hit1 = transformedBox->hit(ray1, 0, 20);
    
    RayTracer::Ray ray2(
        Math::Point3D(Math::Coords{-5, 0, 0}),
        Math::Vector3D(Math::Coords{0, 1, 0}).normalize());
    auto hit2 = transformedBox->hit(ray2, 0, 20);
    
    if (hit1.has_value()) {
        EXPECT_GT(hit1->distance, 0);
        EXPECT_NEAR(hit1->normal.length(), 1.0, 0.001);
    }
    
    if (hit2.has_value()) {
        EXPECT_GT(hit2->distance, 0);
        EXPECT_NEAR(hit2->normal.length(), 1.0, 0.001);
    }
}

TEST_F(BoxTest, GetSourceFileTest) {
    std::string sourceFile = "test_source_file.cpp";
    boxDefault->setSourceFile(sourceFile);
    EXPECT_EQ(boxDefault->getSourceFile(), sourceFile);
}

} // namespace RayTracerTest