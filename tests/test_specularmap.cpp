// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for SpecularMap class
*/

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <limits>
#include "Math/Vector2D/Vector2D.hpp"
#include "Texture/SpecularMap/SpecularMap.hpp"

namespace RayTracerTest {

class SpecularMapTest : public ::testing::Test {
 protected:
    void SetUp() override {
        specularMapPath = "/home/alex/Raytracer/assets/textures/specular_map.jpg";
        
        mapWithFilter = std::make_shared<RayTracer::SpecularMap>(specularMapPath, true);
        
        mapNoFilter = std::make_shared<RayTracer::SpecularMap>(specularMapPath, false);
        
        invalidMap = std::make_shared<RayTracer::SpecularMap>("nonexistent_path.jpg");
    }

    std::string specularMapPath;
    std::shared_ptr<RayTracer::SpecularMap> mapWithFilter;
    std::shared_ptr<RayTracer::SpecularMap> mapNoFilter;
    std::shared_ptr<RayTracer::SpecularMap> invalidMap;
};

TEST_F(SpecularMapTest, CreationTest) {
    EXPECT_NE(mapWithFilter, nullptr);
    EXPECT_NE(mapNoFilter, nullptr);
    EXPECT_NE(invalidMap, nullptr);
}

TEST_F(SpecularMapTest, GetSpecularAtTest) {
    Math::Vector2D uvCenter(0.5, 0.5);
    Math::Vector2D uvTopLeft(0.0, 0.0);
    Math::Vector2D uvBottomRight(1.0, 1.0);
    
    double specularCenter = mapWithFilter->getSpecularAt(uvCenter);
    EXPECT_GE(specularCenter, 0.0);
    EXPECT_LE(specularCenter, 1.0);

    double specularTopLeft = mapWithFilter->getSpecularAt(uvTopLeft);
    EXPECT_GE(specularTopLeft, 0.0);
    EXPECT_LE(specularTopLeft, 1.0);
    
    double specularBottomRight = mapWithFilter->getSpecularAt(uvBottomRight);
    EXPECT_GE(specularBottomRight, 0.0);
    EXPECT_LE(specularBottomRight, 1.0);
}

TEST_F(SpecularMapTest, FilteringTest) {
    Math::Vector2D uv(0.25, 0.75);
    
    double specularWithFilter = mapWithFilter->getSpecularAt(uv);
    double specularNoFilter = mapNoFilter->getSpecularAt(uv);
    
    EXPECT_GE(specularWithFilter, 0.0);
    EXPECT_LE(specularWithFilter, 1.0);
    EXPECT_GE(specularNoFilter, 0.0);
    EXPECT_LE(specularNoFilter, 1.0);
}

TEST_F(SpecularMapTest, WrapUVTest) {
    Math::Vector2D uvOutsidePositive(1.2, 1.5);
    Math::Vector2D uvOutsideNegative(-0.2, -0.5);
    
    double specularPositive = mapWithFilter->getSpecularAt(uvOutsidePositive);
    double specularNegative = mapWithFilter->getSpecularAt(uvOutsideNegative);
    
    EXPECT_GE(specularPositive, 0.0);
    EXPECT_LE(specularPositive, 1.0);
    EXPECT_GE(specularNegative, 0.0);
    EXPECT_LE(specularNegative, 1.0);
}

TEST_F(SpecularMapTest, CloneTest) {
    auto clonedMap = mapWithFilter->clone();
    
    EXPECT_NE(clonedMap, nullptr);
    
    Math::Vector2D uv(0.5, 0.5);
    double originalSpecular = mapWithFilter->getSpecularAt(uv);
    double clonedSpecular = clonedMap->getSpecularAt(uv);
    
    EXPECT_DOUBLE_EQ(clonedSpecular, originalSpecular);
}

TEST_F(SpecularMapTest, DefaultMapTest) {
    Math::Vector2D uv(0.5, 0.5);
    double specular = invalidMap->getSpecularAt(uv);
    
    EXPECT_GE(specular, 0.0);
    EXPECT_LE(specular, 1.0);
}

TEST_F(SpecularMapTest, EdgeCasesTest) {
    Math::Vector2D uvBoundary1(0.999, 0.999);
    Math::Vector2D uvBoundary2(0.001, 0.001);
    
    double specularBoundary1 = mapWithFilter->getSpecularAt(uvBoundary1);
    double specularBoundary2 = mapWithFilter->getSpecularAt(uvBoundary2);
    
    EXPECT_GE(specularBoundary1, 0.0);
    EXPECT_LE(specularBoundary1, 1.0);
    EXPECT_GE(specularBoundary2, 0.0);
    EXPECT_LE(specularBoundary2, 1.0);
}

TEST_F(SpecularMapTest, InterpolationTest) {
    Math::Vector2D uvInterpolate(0.33, 0.67);
    
    double specularWithFilter = mapWithFilter->getSpecularAt(uvInterpolate);
    double specularNoFilter = mapNoFilter->getSpecularAt(uvInterpolate);
    
    EXPECT_GE(specularWithFilter, 0.0);
    EXPECT_LE(specularWithFilter, 1.0);
    EXPECT_GE(specularNoFilter, 0.0);
    EXPECT_LE(specularNoFilter, 1.0);
}

}  // namespace RayTracerTest