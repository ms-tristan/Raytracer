/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for Material class
*/

#include <gtest/gtest.h>
#include <memory>
#include "Material/Material.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Vector2D/Vector2D.hpp"
#include "Texture/ProceduralTexture/ChessboardTexture.hpp"
#include "Texture/NormalMap/NormalMap.hpp"
#include "Texture/DisplacementMap/DisplacementMap.hpp"
#include "Texture/SpecularMap/SpecularMap.hpp"
#include "Texture/AmbientOcclusionMap/AmbientOcclusionMap.hpp"

namespace RayTracerTest {

class MaterialTest : public ::testing::Test {
 protected:
    void SetUp() override {
        materialBasic = std::make_shared<RayTracer::Material>(
            Math::Vector3D(Math::Coords{0.8, 0.3, 0.2}),
            0.5,
            0.2,
            1.5,
            25.0
        );

        auto chessboardTexture = std::make_shared<RayTracer::ChessboardTexture>(
            Math::Vector3D(Math::Coords{0.2, 0.8, 0.3}),
            Math::Vector3D(Math::Coords{0.9, 0.9, 0.9}),
            2.0
        );

        materialWithTexture = std::make_shared<RayTracer::Material>(
            Math::Vector3D(Math::Coords{0.5, 0.5, 0.5}),
            chessboardTexture,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            0.3,
            0.0,
            1.0,
            10.0
        );

        materialDefault = std::make_shared<RayTracer::Material>();
    }

    std::shared_ptr<RayTracer::Material> materialBasic;
    std::shared_ptr<RayTracer::Material> materialWithTexture;
    std::shared_ptr<RayTracer::Material> materialDefault;
};

TEST_F(MaterialTest, ConstructorTest) {
    EXPECT_NEAR(materialBasic->color.X, 0.8, 0.001);
    EXPECT_NEAR(materialBasic->color.Y, 0.3, 0.001);
    EXPECT_NEAR(materialBasic->color.Z, 0.2, 0.001);
    EXPECT_NEAR(materialBasic->reflectivity, 0.5, 0.001);
    EXPECT_NEAR(materialBasic->transparency, 0.2, 0.001);
    EXPECT_NEAR(materialBasic->refractionIndex, 1.5, 0.001);
    EXPECT_NEAR(materialBasic->shininess, 25.0, 0.001);
    EXPECT_FALSE(materialBasic->hasTexture());
    EXPECT_FALSE(materialBasic->hasNormalMap());
    EXPECT_FALSE(materialBasic->hasDisplacementMap());
    EXPECT_FALSE(materialBasic->hasSpecularMap());
    EXPECT_FALSE(materialBasic->hasAmbientOcclusionMap());

    EXPECT_NEAR(materialWithTexture->color.X, 0.5, 0.001);
    EXPECT_NEAR(materialWithTexture->color.Y, 0.5, 0.001);
    EXPECT_NEAR(materialWithTexture->color.Z, 0.5, 0.001);
    EXPECT_NEAR(materialWithTexture->reflectivity, 0.3, 0.001);
    EXPECT_NEAR(materialWithTexture->transparency, 0.0, 0.001);
    EXPECT_NEAR(materialWithTexture->refractionIndex, 1.0, 0.001);
    EXPECT_NEAR(materialWithTexture->shininess, 10.0, 0.001);
    EXPECT_TRUE(materialWithTexture->hasTexture());
    EXPECT_FALSE(materialWithTexture->hasNormalMap());
    EXPECT_FALSE(materialWithTexture->hasDisplacementMap());
    EXPECT_FALSE(materialWithTexture->hasSpecularMap());
    EXPECT_FALSE(materialWithTexture->hasAmbientOcclusionMap());

    EXPECT_NEAR(materialDefault->color.X, 1.0, 0.001);
    EXPECT_NEAR(materialDefault->color.Y, 1.0, 0.001);
    EXPECT_NEAR(materialDefault->color.Z, 1.0, 0.001);
    EXPECT_NEAR(materialDefault->reflectivity, 0.0, 0.001);
    EXPECT_NEAR(materialDefault->transparency, 0.0, 0.001);
    EXPECT_NEAR(materialDefault->refractionIndex, 1.0, 0.001);
    EXPECT_NEAR(materialDefault->shininess, 10.0, 0.001);
    EXPECT_FALSE(materialDefault->hasTexture());
}

TEST_F(MaterialTest, GetColorAtTest) {
    Math::Vector2D uv1(0.25, 0.75);
    Math::Vector2D uv2(0.5, 0.5);
    
    auto color1 = materialBasic->getColorAt(uv1);
    auto color2 = materialBasic->getColorAt(uv2);
    
    EXPECT_NEAR(color1.X, materialBasic->color.X, 0.001);
    EXPECT_NEAR(color1.Y, materialBasic->color.Y, 0.001);
    EXPECT_NEAR(color1.Z, materialBasic->color.Z, 0.001);
    
    EXPECT_NEAR(color2.X, materialBasic->color.X, 0.001);
    EXPECT_NEAR(color2.Y, materialBasic->color.Y, 0.001);
    EXPECT_NEAR(color2.Z, materialBasic->color.Z, 0.001);
    
    auto colorWithTexture1 = materialWithTexture->getColorAt(uv1);
    auto colorWithTexture2 = materialWithTexture->getColorAt(uv2);
    
    EXPECT_LE(colorWithTexture1.X, 1.0);
    EXPECT_LE(colorWithTexture1.Y, 1.0);
    EXPECT_LE(colorWithTexture1.Z, 1.0);
}

TEST_F(MaterialTest, CloneTest) {
    auto clonedMaterial = materialBasic->clone();
    
    EXPECT_NEAR(clonedMaterial->color.X, materialBasic->color.X, 0.001);
    EXPECT_NEAR(clonedMaterial->color.Y, materialBasic->color.Y, 0.001);
    EXPECT_NEAR(clonedMaterial->color.Z, materialBasic->color.Z, 0.001);
    EXPECT_NEAR(clonedMaterial->reflectivity, materialBasic->reflectivity, 0.001);
    EXPECT_NEAR(clonedMaterial->transparency, materialBasic->transparency, 0.001);
    EXPECT_NEAR(clonedMaterial->refractionIndex, materialBasic->refractionIndex, 0.001);
    EXPECT_NEAR(clonedMaterial->shininess, materialBasic->shininess, 0.001);

    auto clonedWithTexture = materialWithTexture->clone();
    
    EXPECT_TRUE(clonedWithTexture->hasTexture());
    EXPECT_FALSE(clonedWithTexture->hasNormalMap());

    Math::Vector2D uv(0.5, 0.5);
    auto originalColor = materialWithTexture->getColorAt(uv);
    auto clonedColor = clonedWithTexture->getColorAt(uv);
    
    EXPECT_NEAR(clonedColor.X, originalColor.X, 0.001);
    EXPECT_NEAR(clonedColor.Y, originalColor.Y, 0.001);
    EXPECT_NEAR(clonedColor.Z, originalColor.Z, 0.001);
}

TEST_F(MaterialTest, SetTextureMapsTest) {
    auto material = std::make_shared<RayTracer::Material>();
    
    auto chessboardTexture = std::make_shared<RayTracer::ChessboardTexture>(
        Math::Vector3D(Math::Coords{0.1, 0.2, 0.3}),
        Math::Vector3D(Math::Coords{0.8, 0.8, 0.8}),
        1.0
    );
    material->setTexture(chessboardTexture);
    EXPECT_TRUE(material->hasTexture());
    
    auto normalMap = std::make_shared<RayTracer::NormalMap>("dummy_path.png");
    material->setNormalMap(normalMap);
    EXPECT_TRUE(material->hasNormalMap());
    
    auto displacementMap = std::make_shared<RayTracer::DisplacementMap>("dummy_path.png");
    material->setDisplacementMap(displacementMap);
    EXPECT_TRUE(material->hasDisplacementMap());
    
    auto specularMap = std::make_shared<RayTracer::SpecularMap>("dummy_path.png");
    material->setSpecularMap(specularMap);
    EXPECT_TRUE(material->hasSpecularMap());
    
    auto aoMap = std::make_shared<RayTracer::AmbientOcclusionMap>("dummy_path.png");
    material->setAmbientOcclusionMap(aoMap);
    EXPECT_TRUE(material->hasAmbientOcclusionMap());
    
    Math::Vector2D uv(0.5, 0.5);
    Math::Vector3D originalNormal(Math::Coords{0, 1, 0});
    Math::Vector3D modifiedNormal = material->getNormalAt(uv, originalNormal);
    
    EXPECT_GT(modifiedNormal.length(), 0.0);
    
    EXPECT_NO_THROW(material->getDisplacementAt(uv));
    EXPECT_NO_THROW(material->getSpecularAt(uv));
    EXPECT_NO_THROW(material->getAmbientOcclusionAt(uv));
    
    Math::Point3D point(Math::Coords{1, 2, 3});
    Math::Vector3D normal(Math::Coords{0, 1, 0});
    Math::Point3D displacedPoint = material->displacePoint(point, normal, uv);
    
    EXPECT_TRUE(true);
}

} // namespace RayTracerTest