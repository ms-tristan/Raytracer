/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for DisplacementMap class
*/

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <limits>
#include "Math/Vector2D/Vector2D.hpp"
#include "Texture/DisplacementMap/DisplacementMap.hpp"

namespace RayTracerTest {

class DisplacementMapTest : public ::testing::Test {
 protected:
    void SetUp() override {
        displacementMapPath = "/home/alex/Raytracer/assets/textures/displacement_map.jpg";
        mapWithFilter = std::make_shared<RayTracer::DisplacementMap>(displacementMapPath, 1.0, true);
        mapNoFilter = std::make_shared<RayTracer::DisplacementMap>(displacementMapPath, 1.0, false);
        invalidMap = std::make_shared<RayTracer::DisplacementMap>("nonexistent_path.jpg", 1.0);
        lowIntensityMap = std::make_shared<RayTracer::DisplacementMap>(displacementMapPath, 0.5, true);
    }

    std::string displacementMapPath;
    std::shared_ptr<RayTracer::DisplacementMap> mapWithFilter;
    std::shared_ptr<RayTracer::DisplacementMap> mapNoFilter;
    std::shared_ptr<RayTracer::DisplacementMap> invalidMap;
    std::shared_ptr<RayTracer::DisplacementMap> lowIntensityMap;
};

TEST_F(DisplacementMapTest, CreationTest) {
    EXPECT_NE(mapWithFilter, nullptr);
    EXPECT_NE(mapNoFilter, nullptr);
    EXPECT_NE(invalidMap, nullptr);
    EXPECT_NE(lowIntensityMap, nullptr);
}

TEST_F(DisplacementMapTest, GetDisplacementAtTest) {
    Math::Vector2D uvCenter(0.5, 0.5);
    Math::Vector2D uvTopLeft(0.0, 0.0);
    Math::Vector2D uvBottomRight(1.0, 1.0);

    double displacementCenter = mapWithFilter->getDisplacementAt(uvCenter);
    EXPECT_GE(displacementCenter, 0.0);
    EXPECT_LE(displacementCenter, 1.0);

    double displacementTopLeft = mapWithFilter->getDisplacementAt(uvTopLeft);
    EXPECT_GE(displacementTopLeft, 0.0);
    EXPECT_LE(displacementTopLeft, 1.0);
    
    double displacementBottomRight = mapWithFilter->getDisplacementAt(uvBottomRight);
    EXPECT_GE(displacementBottomRight, 0.0);
    EXPECT_LE(displacementBottomRight, 1.0);
}

TEST_F(DisplacementMapTest, FilteringTest) {
    Math::Vector2D uv(0.25, 0.75);
    
    double displacementWithFilter = mapWithFilter->getDisplacementAt(uv);
    double displacementNoFilter = mapNoFilter->getDisplacementAt(uv);
    EXPECT_GE(displacementWithFilter, 0.0);
    EXPECT_LE(displacementWithFilter, 1.0);
    EXPECT_GE(displacementNoFilter, 0.0);
    EXPECT_LE(displacementNoFilter, 1.0);
}

TEST_F(DisplacementMapTest, WrapUVTest) {
    Math::Vector2D uvOutsidePositive(1.2, 1.5);
    Math::Vector2D uvOutsideNegative(-0.2, -0.5);
    
    double displacementPositive = mapWithFilter->getDisplacementAt(uvOutsidePositive);
    double displacementNegative = mapWithFilter->getDisplacementAt(uvOutsideNegative);
    EXPECT_GE(displacementPositive, 0.0);
    EXPECT_LE(displacementPositive, 1.0);
    EXPECT_GE(displacementNegative, 0.0);
    EXPECT_LE(displacementNegative, 1.0);
}

TEST_F(DisplacementMapTest, IntensityTest) {
    std::vector<Math::Vector2D> testPoints = {
        Math::Vector2D(0.2, 0.2),
        Math::Vector2D(0.4, 0.6),
        Math::Vector2D(0.5, 0.5),
        Math::Vector2D(0.7, 0.8),
        Math::Vector2D(0.9, 0.1)
    };
    
    bool foundDifference = false;
    
    for (const auto& uv : testPoints) {
        double displacementFull = mapWithFilter->getDisplacementAt(uv);
        double displacementLow = lowIntensityMap->getDisplacementAt(uv);
        EXPECT_GE(displacementFull, 0.0);
        EXPECT_LE(displacementFull, 1.0);
        EXPECT_GE(displacementLow, 0.0);
        EXPECT_LE(displacementLow, 1.0);
        if (std::abs(displacementFull - displacementLow) > 1e-6) {
            foundDifference = true;
            break;
        }
    }
}

TEST_F(DisplacementMapTest, CloneTest) {
    auto clonedMap = mapWithFilter->clone();
    
    EXPECT_NE(clonedMap, nullptr);
    Math::Vector2D uv(0.5, 0.5);
    double originalDisplacement = mapWithFilter->getDisplacementAt(uv);
    double clonedDisplacement = clonedMap->getDisplacementAt(uv);
    
    EXPECT_DOUBLE_EQ(clonedDisplacement, originalDisplacement);
}

TEST_F(DisplacementMapTest, TypeTest) {
    EXPECT_EQ(mapWithFilter->getType(), "DisplacementMap");
}

TEST_F(DisplacementMapTest, DefaultMapTest) {
    Math::Vector2D uv(0.5, 0.5);
    double displacement = invalidMap->getDisplacementAt(uv);
    EXPECT_GE(displacement, 0.0);
    EXPECT_LE(displacement, 1.0);
}

TEST_F(DisplacementMapTest, EdgeCasesTest) {
    Math::Vector2D uvBoundary1(0.999, 0.999);
    Math::Vector2D uvBoundary2(0.001, 0.001);
    double displacementBoundary1 = mapWithFilter->getDisplacementAt(uvBoundary1);
    double displacementBoundary2 = mapWithFilter->getDisplacementAt(uvBoundary2);
    EXPECT_GE(displacementBoundary1, 0.0);
    EXPECT_LE(displacementBoundary1, 1.0);
    EXPECT_GE(displacementBoundary2, 0.0);
    EXPECT_LE(displacementBoundary2, 1.0);
}

}  // namespace RayTracerTest