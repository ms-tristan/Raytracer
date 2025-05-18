/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for NormalMap class
*/

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "Math/Vector2D/Vector2D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Texture/NormalMap/NormalMap.hpp"

namespace RayTracerTest {

class NormalMapTest : public ::testing::Test {
 protected:
    void SetUp() override {
        normalMapPath = "/home/alex/Raytracer/assets/textures/normal_map.jpg";
        
        mapWithFilter = std::make_shared<RayTracer::NormalMap>(normalMapPath, 1.0, true);
        
        mapNoFilter = std::make_shared<RayTracer::NormalMap>(normalMapPath, 1.0, false);
        
        invalidMap = std::make_shared<RayTracer::NormalMap>("nonexistent_path.jpg", 1.0);

        lowStrengthMap = std::make_shared<RayTracer::NormalMap>(normalMapPath, 0.5, true);
    }

    std::string normalMapPath;
    std::shared_ptr<RayTracer::NormalMap> mapWithFilter;
    std::shared_ptr<RayTracer::NormalMap> mapNoFilter;
    std::shared_ptr<RayTracer::NormalMap> invalidMap;
    std::shared_ptr<RayTracer::NormalMap> lowStrengthMap;
};

TEST_F(NormalMapTest, CreationTest) {
    EXPECT_NE(mapWithFilter, nullptr);
    EXPECT_NE(mapNoFilter, nullptr);
    EXPECT_NE(invalidMap, nullptr);
    EXPECT_NE(lowStrengthMap, nullptr);
}

TEST_F(NormalMapTest, GetNormalAtTest) {
    Math::Vector2D uvCenter(0.5, 0.5);
    Math::Vector2D uvTopLeft(0.0, 0.0);
    Math::Vector2D uvBottomRight(1.0, 1.0);
    
    Math::Vector3D normalCenter = mapWithFilter->getNormalAt(uvCenter);
    EXPECT_NEAR(normalCenter.length(), 1.0, 0.001);

    Math::Vector3D normalTopLeft = mapWithFilter->getNormalAt(uvTopLeft);
    EXPECT_NEAR(normalTopLeft.length(), 1.0, 0.001);
    
    Math::Vector3D normalBottomRight = mapWithFilter->getNormalAt(uvBottomRight);
    EXPECT_NEAR(normalBottomRight.length(), 1.0, 0.001);
}

TEST_F(NormalMapTest, FilteringTest) {
    Math::Vector2D uv(0.25, 0.75);
    
    Math::Vector3D normalWithFilter = mapWithFilter->getNormalAt(uv);
    Math::Vector3D normalNoFilter = mapNoFilter->getNormalAt(uv);
    
    EXPECT_NEAR(normalWithFilter.length(), 1.0, 0.001);
    EXPECT_NEAR(normalNoFilter.length(), 1.0, 0.001);
}

TEST_F(NormalMapTest, WrapUVTest) {
    Math::Vector2D uvOutsidePositive(1.2, 1.5);
    Math::Vector2D uvOutsideNegative(-0.2, -0.5);
    
    Math::Vector3D normalPositive = mapWithFilter->getNormalAt(uvOutsidePositive);
    Math::Vector3D normalNegative = mapWithFilter->getNormalAt(uvOutsideNegative);
    
    EXPECT_NEAR(normalPositive.length(), 1.0, 0.001);
    EXPECT_NEAR(normalNegative.length(), 1.0, 0.001);
}

TEST_F(NormalMapTest, StrengthTest) {
    Math::Vector2D uv(0.5, 0.5);
    
    Math::Vector3D normalFull = mapWithFilter->getNormalAt(uv);
    Math::Vector3D normalLow = lowStrengthMap->getNormalAt(uv);
    
    EXPECT_NEAR(normalFull.length(), 1.0, 0.001);
    EXPECT_NEAR(normalLow.length(), 1.0, 0.001);
    
    EXPECT_DOUBLE_EQ(mapWithFilter->getStrength(), 1.0);
    EXPECT_DOUBLE_EQ(lowStrengthMap->getStrength(), 0.5);
    
    lowStrengthMap->setStrength(0.75);
    EXPECT_DOUBLE_EQ(lowStrengthMap->getStrength(), 0.75);
}

TEST_F(NormalMapTest, CloneTest) {
    auto clonedMap = mapWithFilter->clone();
    
    EXPECT_NE(clonedMap, nullptr);
    
    Math::Vector2D uv(0.5, 0.5);
    Math::Vector3D originalNormal = mapWithFilter->getNormalAt(uv);
    Math::Vector3D clonedNormal = clonedMap->getNormalAt(uv);
    
    EXPECT_NEAR(clonedNormal.X, originalNormal.X, 0.001);
    EXPECT_NEAR(clonedNormal.Y, originalNormal.Y, 0.001);
    EXPECT_NEAR(clonedNormal.Z, originalNormal.Z, 0.001);
}

TEST_F(NormalMapTest, TypeTest) {
    EXPECT_EQ(mapWithFilter->getType(), "NormalMap");
}

TEST_F(NormalMapTest, EdgeCasesTest) {
    Math::Vector2D uvBoundary1(0.999, 0.999);
    Math::Vector2D uvBoundary2(0.001, 0.001);
    
    Math::Vector3D normalBoundary1 = mapWithFilter->getNormalAt(uvBoundary1);
    Math::Vector3D normalBoundary2 = mapWithFilter->getNormalAt(uvBoundary2);
    
    EXPECT_NEAR(normalBoundary1.length(), 1.0, 0.001);
    EXPECT_NEAR(normalBoundary2.length(), 1.0, 0.001);
}

}  // namespace RayTracerTest