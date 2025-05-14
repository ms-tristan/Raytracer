/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for ImageTexture class
*/

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "Math/Vector2D/Vector2D.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Texture/ImageTexture/ImageTexture.hpp"

namespace RayTracerTest {

class ImageTextureTest : public ::testing::Test {
 protected:
    void SetUp() override {
        texturePath = "/home/alex/Raytracer/assets/textures/brick.jpg";
        textureWithFilter = std::make_shared<RayTracer::ImageTexture>(texturePath, true);
        textureNoFilter = std::make_shared<RayTracer::ImageTexture>(texturePath, false);
        invalidTexture = std::make_shared<RayTracer::ImageTexture>("nonexistent_path.jpg");
    }

    std::string texturePath;
    std::shared_ptr<RayTracer::ImageTexture> textureWithFilter;
    std::shared_ptr<RayTracer::ImageTexture> textureNoFilter;
    std::shared_ptr<RayTracer::ImageTexture> invalidTexture;
};

TEST_F(ImageTextureTest, CreationTest) {
    EXPECT_NE(textureWithFilter, nullptr);
    EXPECT_NE(textureNoFilter, nullptr);
    EXPECT_NE(invalidTexture, nullptr);
    Math::Vector3D zeroVec(Math::Coords{0, 0, 0});
    Math::Vector3D invalidColor = invalidTexture->getColorAt(Math::Vector2D(0.5, 0.5));
    bool hasNonZeroComponent = (invalidColor.X != 0.0) || 
                               (invalidColor.Y != 0.0) || 
                               (invalidColor.Z != 0.0);
    EXPECT_TRUE(hasNonZeroComponent);
}

TEST_F(ImageTextureTest, GetColorTest) {
    Math::Vector2D uvCenter(0.5, 0.5);
    Math::Vector2D uvTopLeft(0.0, 0.0);
    Math::Vector2D uvBottomRight(1.0, 1.0);
    Math::Vector3D colorCenter = textureWithFilter->getColorAt(uvCenter);
    EXPECT_GE(colorCenter.X, 0.0);
    EXPECT_GE(colorCenter.Y, 0.0);
    EXPECT_GE(colorCenter.Z, 0.0);
    EXPECT_LE(colorCenter.X, 1.0);
    EXPECT_LE(colorCenter.Y, 1.0);
    EXPECT_LE(colorCenter.Z, 1.0);
    Math::Vector3D colorTopLeft = textureWithFilter->getColorAt(uvTopLeft);
    Math::Vector3D colorBottomRight = textureWithFilter->getColorAt(uvBottomRight);
    bool colorsDifferent = (colorCenter.X != colorTopLeft.X) || 
                          (colorCenter.Y != colorTopLeft.Y) || 
                          (colorCenter.Z != colorTopLeft.Z) || 
                          (colorCenter.X != colorBottomRight.X) || 
                          (colorCenter.Y != colorBottomRight.Y) || 
                          (colorCenter.Z != colorBottomRight.Z);
    EXPECT_TRUE(colorsDifferent);
}

TEST_F(ImageTextureTest, FilteringTest) {
    Math::Vector2D uv(0.25, 0.75);
    Math::Vector3D colorWithFilter = textureWithFilter->getColorAt(uv);
    Math::Vector3D colorNoFilter = textureNoFilter->getColorAt(uv);
    EXPECT_GE(colorWithFilter.X, 0.0);
    EXPECT_LE(colorWithFilter.X, 1.0);
    EXPECT_GE(colorNoFilter.X, 0.0);
    EXPECT_LE(colorNoFilter.X, 1.0);
}

TEST_F(ImageTextureTest, WrapUVTest) {
    Math::Vector2D uvOutsidePositive(1.2, 1.5);
    Math::Vector2D uvOutsideNegative(-0.2, -0.5);
    Math::Vector3D colorPositive = textureWithFilter->getColorAt(uvOutsidePositive);
    Math::Vector3D colorNegative = textureWithFilter->getColorAt(uvOutsideNegative);
    EXPECT_GE(colorPositive.X, 0.0);
    EXPECT_LE(colorPositive.X, 1.0);
    EXPECT_GE(colorNegative.X, 0.0);
    EXPECT_LE(colorNegative.X, 1.0);
}

TEST_F(ImageTextureTest, CloneTest) {
    auto clonedTexture = textureWithFilter->clone();
    EXPECT_NE(clonedTexture, nullptr);
    auto castClone = std::dynamic_pointer_cast<RayTracer::ImageTexture>(clonedTexture);
    EXPECT_NE(castClone, nullptr);
    Math::Vector2D uv(0.5, 0.5);
    Math::Vector3D colorOriginal = textureWithFilter->getColorAt(uv);
    Math::Vector3D colorClone = clonedTexture->getColorAt(uv);
    EXPECT_EQ(colorOriginal.X, colorClone.X);
    EXPECT_EQ(colorOriginal.Y, colorClone.Y);
    EXPECT_EQ(colorOriginal.Z, colorClone.Z);
}

TEST_F(ImageTextureTest, TypeNameTest) {
    EXPECT_EQ(RayTracer::ImageTexture::getTypeNameStatic(), "image");
}

TEST_F(ImageTextureTest, EdgeCaseUVTest) {
    Math::Vector2D uvZero(0.0, 0.0);
    Math::Vector2D uvOne(1.0, 1.0);
    Math::Vector2D uvHalfZero(0.5, 0.0);
    Math::Vector2D uvZeroHalf(0.0, 0.5);
    Math::Vector2D uvNaN(std::numeric_limits<double>::quiet_NaN(), 0.5);
    Math::Vector2D uvInf(std::numeric_limits<double>::infinity(), 0.5);
    Math::Vector3D colorZero = textureWithFilter->getColorAt(uvZero);
    Math::Vector3D colorOne = textureWithFilter->getColorAt(uvOne);
    Math::Vector3D colorHalfZero = textureWithFilter->getColorAt(uvHalfZero);
    Math::Vector3D colorZeroHalf = textureWithFilter->getColorAt(uvZeroHalf);
    EXPECT_GE(colorZero.X, 0.0);
    EXPECT_LE(colorZero.X, 1.0);
    EXPECT_GE(colorOne.X, 0.0);
    EXPECT_LE(colorOne.X, 1.0);
    EXPECT_GE(colorHalfZero.X, 0.0);
    EXPECT_LE(colorHalfZero.X, 1.0);
    EXPECT_GE(colorZeroHalf.X, 0.0);
    EXPECT_LE(colorZeroHalf.X, 1.0);
    Math::Vector3D colorNaN = textureWithFilter->getColorAt(uvNaN);
    Math::Vector3D colorInf = textureWithFilter->getColorAt(uvInf);
    EXPECT_GE(colorNaN.X, 0.0);
    EXPECT_LE(colorNaN.X, 1.0);
    EXPECT_GE(colorInf.X, 0.0);
    EXPECT_LE(colorInf.X, 1.0);
}

TEST_F(ImageTextureTest, EmptyImageTest) {
    auto emptyTexture = std::make_shared<RayTracer::ImageTexture>("");
    Math::Vector2D uv(0.5, 0.5);
    Math::Vector3D color = emptyTexture->getColorAt(uv);
    EXPECT_GE(color.X, 0.0);
    EXPECT_LE(color.X, 1.0);
    EXPECT_GE(color.Y, 0.0);
    EXPECT_LE(color.Y, 1.0);
    EXPECT_GE(color.Z, 0.0);
    EXPECT_LE(color.Z, 1.0);
}

TEST_F(ImageTextureTest, MultipleAccessTest) {
    Math::Vector2D uv(0.33, 0.67);
    Math::Vector3D color1 = textureWithFilter->getColorAt(uv);
    Math::Vector3D color2 = textureWithFilter->getColorAt(uv);
    Math::Vector3D color3 = textureWithFilter->getColorAt(uv);
    EXPECT_EQ(color1.X, color2.X);
    EXPECT_EQ(color1.Y, color2.Y);
    EXPECT_EQ(color1.Z, color2.Z);
    EXPECT_EQ(color1.X, color3.X);
    EXPECT_EQ(color1.Y, color3.Y);
    EXPECT_EQ(color1.Z, color3.Z);
}

TEST_F(ImageTextureTest, ExtremeFiltering) {
    auto textureAlwaysFilter = std::make_shared<RayTracer::ImageTexture>(texturePath, true);
    auto textureNeverFilter = std::make_shared<RayTracer::ImageTexture>(texturePath, false);
    Math::Vector2D uvBetweenPixels(0.123, 0.456);
    Math::Vector3D colorFiltered = textureAlwaysFilter->getColorAt(uvBetweenPixels);
    Math::Vector3D colorUnfiltered = textureNeverFilter->getColorAt(uvBetweenPixels);
    EXPECT_GE(colorFiltered.X, 0.0);
    EXPECT_LE(colorFiltered.X, 1.0);
    EXPECT_GE(colorUnfiltered.X, 0.0);
    EXPECT_LE(colorUnfiltered.X, 1.0);
    bool foundDifferentColors = false;
    for (double u = 0.01; u < 1.0; u += 0.1) {
        for (double v = 0.01; v < 1.0; v += 0.1) {
            Math::Vector2D uvSample(u, v);
            Math::Vector3D colorF = textureAlwaysFilter->getColorAt(uvSample);
            Math::Vector3D colorU = textureNeverFilter->getColorAt(uvSample);
            if (abs(colorF.X - colorU.X) > 0.01 || 
                abs(colorF.Y - colorU.Y) > 0.01 || 
                abs(colorF.Z - colorU.Z) > 0.01) {
                foundDifferentColors = true;
                break;
            }
        }
        if (foundDifferentColors) break;
    }
}

TEST_F(ImageTextureTest, SerializationFunctionalityTest) {
    auto clonedTexture = textureWithFilter->clone();
    EXPECT_NE(clonedTexture, nullptr);
    auto castClone = std::dynamic_pointer_cast<RayTracer::ImageTexture>(clonedTexture);
    EXPECT_NE(castClone, nullptr);
    Math::Vector2D testUV(0.5, 0.5);
    Math::Vector3D originalColor = textureWithFilter->getColorAt(testUV);
    Math::Vector3D cloneColor = clonedTexture->getColorAt(testUV);
    EXPECT_NEAR(originalColor.X, cloneColor.X, 0.001);
    EXPECT_NEAR(originalColor.Y, cloneColor.Y, 0.001);
    EXPECT_NEAR(originalColor.Z, cloneColor.Z, 0.001);
}

} // namespace RayTracerTest