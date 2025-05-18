/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for PerlinNoiseTexture class
*/

#include <gtest/gtest.h>
#include <memory>
#include <cmath>
#include "Math/Vector2D/Vector2D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Texture/ProceduralTexture/PerlinNoiseTexture.hpp"

namespace RayTracerTest {

class PerlinNoiseTextureTest : public ::testing::Test {
 protected:
    void SetUp() override {
        perlinDefault = std::make_shared<RayTracer::PerlinNoiseTexture>();
        
        Math::Vector3D color1(Math::Coords{0.8, 0.2, 0.2});
        Math::Vector3D color2(Math::Coords{0.2, 0.8, 0.2});
        perlinCustom = std::make_shared<RayTracer::PerlinNoiseTexture>(
            color1, color2, 2.0, 0.7, 8, 123);
    }

    std::shared_ptr<RayTracer::PerlinNoiseTexture> perlinDefault;
    std::shared_ptr<RayTracer::PerlinNoiseTexture> perlinCustom;
};

TEST_F(PerlinNoiseTextureTest, CreationTest) {
    EXPECT_NE(perlinDefault, nullptr);
    EXPECT_NE(perlinCustom, nullptr);
    
    EXPECT_EQ(perlinCustom->getPersistence(), 0.7);
    EXPECT_EQ(perlinCustom->getOctaves(), 8);
    EXPECT_EQ(perlinCustom->getScale(), 2.0);
}

TEST_F(PerlinNoiseTextureTest, GetColorTest) {
    Math::Vector2D uvCenter(0.5, 0.5);
    Math::Vector2D uvTopLeft(0.0, 0.0);
    Math::Vector2D uvBottomRight(1.0, 1.0);
    
    Math::Vector3D colorCenter = perlinDefault->getColorAt(uvCenter);
    EXPECT_GE(colorCenter.X, 0.0);
    EXPECT_GE(colorCenter.Y, 0.0);
    EXPECT_GE(colorCenter.Z, 0.0);
    EXPECT_LE(colorCenter.X, 1.0);
    EXPECT_LE(colorCenter.Y, 1.0);
    EXPECT_LE(colorCenter.Z, 1.0);
    
    Math::Vector3D colorTopLeft = perlinCustom->getColorAt(uvTopLeft);
    Math::Vector3D colorBottomRight = perlinCustom->getColorAt(uvBottomRight);
    
    EXPECT_GE(colorTopLeft.X, 0.0);
    EXPECT_LE(colorTopLeft.X, 1.0);
    EXPECT_GE(colorBottomRight.X, 0.0);
    EXPECT_LE(colorBottomRight.X, 1.0);
}

TEST_F(PerlinNoiseTextureTest, NoiseConsistencyTest) {
    Math::Vector2D uv(0.3, 0.7);
    
    Math::Vector3D color1 = perlinDefault->getColorAt(uv);
    Math::Vector3D color2 = perlinDefault->getColorAt(uv);
    
    EXPECT_EQ(color1.X, color2.X);
    EXPECT_EQ(color1.Y, color2.Y);
    EXPECT_EQ(color1.Z, color2.Z);
}

TEST_F(PerlinNoiseTextureTest, SetParametersTest) {
    perlinCustom->setPersistence(0.3);
    perlinCustom->setOctaves(4);
    
    EXPECT_EQ(perlinCustom->getPersistence(), 0.3);
    EXPECT_EQ(perlinCustom->getOctaves(), 4);
    
    auto perlin1 = std::make_shared<RayTracer::PerlinNoiseTexture>(
        Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}),
        Math::Vector3D(Math::Coords{0.0, 0.0, 0.0}),
        1.0, 0.1, 6, 42);
        
    auto perlin2 = std::make_shared<RayTracer::PerlinNoiseTexture>(
        Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}),
        Math::Vector3D(Math::Coords{0.0, 0.0, 0.0}),
        1.0, 0.9, 6, 42);
    
    bool foundDifference = false;
    
    for (double u = 0.1; u < 1.0; u += 0.2) {
        for (double v = 0.1; v < 1.0; v += 0.2) {
            Math::Vector2D uv(u, v);
            Math::Vector3D color1 = perlin1->getColorAt(uv);
            Math::Vector3D color2 = perlin2->getColorAt(uv);
            
            if (std::abs(color1.X - color2.X) > 0.01 ||
                std::abs(color1.Y - color2.Y) > 0.01 ||
                std::abs(color1.Z - color2.Z) > 0.01) {
                foundDifference = true;
                break;
            }
        }
        if (foundDifference) break;
    }
    
    EXPECT_TRUE(foundDifference);
}

TEST_F(PerlinNoiseTextureTest, WrapUVTest) {
    Math::Vector2D uvOutsidePositive(1.5, 2.0);
    Math::Vector2D uvOutsideNegative(-0.5, -1.0);
    
    Math::Vector3D colorPositive = perlinDefault->getColorAt(uvOutsidePositive);
    Math::Vector3D colorNegative = perlinDefault->getColorAt(uvOutsideNegative);
    
    EXPECT_GE(colorPositive.X, 0.0);
    EXPECT_LE(colorPositive.X, 1.0);
    EXPECT_GE(colorNegative.X, 0.0);
    EXPECT_LE(colorNegative.X, 1.0);
}

TEST_F(PerlinNoiseTextureTest, DifferentSeedsTest) {
    auto perlin1 = std::make_shared<RayTracer::PerlinNoiseTexture>(
        Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}),
        Math::Vector3D(Math::Coords{0.0, 0.0, 0.0}),
        1.0, 0.5, 6, 42);
        
    auto perlin2 = std::make_shared<RayTracer::PerlinNoiseTexture>(
        Math::Vector3D(Math::Coords{1.0, 1.0, 1.0}),
        Math::Vector3D(Math::Coords{0.0, 0.0, 0.0}),
        1.0, 0.5, 6, 123);
        
    Math::Vector2D uv(0.5, 0.5);
    Math::Vector3D color1 = perlin1->getColorAt(uv);
    Math::Vector3D color2 = perlin2->getColorAt(uv);
    
    bool seedsProduceDifferentColors = (color1.X != color2.X) || 
                                      (color1.Y != color2.Y) || 
                                      (color1.Z != color2.Z);
    EXPECT_TRUE(seedsProduceDifferentColors);
}

TEST_F(PerlinNoiseTextureTest, CloneTest) {
    auto clonedTexture = perlinCustom->clone();
    
    EXPECT_NE(clonedTexture, nullptr);
    
    auto castClone = std::dynamic_pointer_cast<RayTracer::PerlinNoiseTexture>(clonedTexture);
    EXPECT_NE(castClone, nullptr);
    
    if (castClone) {
        EXPECT_EQ(castClone->getPersistence(), perlinCustom->getPersistence());
        EXPECT_EQ(castClone->getOctaves(), perlinCustom->getOctaves());
        EXPECT_EQ(castClone->getScale(), perlinCustom->getScale());
        
        Math::Vector2D uv(0.5, 0.5);
        Math::Vector3D colorOriginal = perlinCustom->getColorAt(uv);
        Math::Vector3D colorClone = clonedTexture->getColorAt(uv);
        
        EXPECT_EQ(colorOriginal.X, colorClone.X);
        EXPECT_EQ(colorOriginal.Y, colorClone.Y);
        EXPECT_EQ(colorOriginal.Z, colorClone.Z);
    }
}

TEST_F(PerlinNoiseTextureTest, TypeNameTest) {
    EXPECT_EQ(RayTracer::PerlinNoiseTexture::getTypeNameStatic(), "perlin");
}

} // namespace RayTracerTest