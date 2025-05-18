/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for APrimitive class
*/

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "Primitive/APrimitive/APrimitive.hpp"
#include "Material/Material.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Point3D/Point3D.hpp"

namespace RayTracer {
class ConcretePrimitive : public APrimitive {
 public:
    ConcretePrimitive() : APrimitive() {}
    explicit ConcretePrimitive(const std::shared_ptr<Material>& material) 
        : APrimitive(material) {}

    void translate(const Math::Vector3D &translation) override {}
    std::optional<HitInfo> hit(const Ray &ray, double tMin, double tMax) override { 
        return std::nullopt; 
    }
    std::shared_ptr<IPrimitive> clone() const override { 
        return std::make_shared<ConcretePrimitive>(*this); 
    }
    void getLibConfigParams(std::shared_ptr<libconfig::Setting> setting) const override {}
    std::string getTypeName() const override { return "ConcreteTestPrimitive"; }
    Math::Point3D getPosition() const { return Math::Point3D(Math::Coords{0.0, 0.0, 0.0}); }
};
}

namespace RayTracerTest {

class APrimitiveTest : public ::testing::Test {
 protected:
    std::shared_ptr<RayTracer::Material> material;
    std::unique_ptr<RayTracer::ConcretePrimitive> primitive;
    std::unique_ptr<RayTracer::ConcretePrimitive> defaultPrimitive;

    void SetUp() override {
        material = std::make_shared<RayTracer::Material>();
        material->color = Math::Vector3D(Math::Coords{1.0, 0.5, 0.3});
        primitive = std::make_unique<RayTracer::ConcretePrimitive>(material);
        defaultPrimitive = std::make_unique<RayTracer::ConcretePrimitive>();
    }
};

TEST_F(APrimitiveTest, DefaultConstructor) {
    ASSERT_NE(nullptr, defaultPrimitive->getMaterial());
    EXPECT_DOUBLE_EQ(0.0, defaultPrimitive->getRotationX());
    EXPECT_DOUBLE_EQ(0.0, defaultPrimitive->getRotationY());
    EXPECT_DOUBLE_EQ(0.0, defaultPrimitive->getRotationZ());
    EXPECT_EQ("", defaultPrimitive->getSourceFile());
}

TEST_F(APrimitiveTest, MaterialConstructor) {
    ASSERT_EQ(material, primitive->getMaterial());
    EXPECT_DOUBLE_EQ(1.0, primitive->getMaterial()->color.X);
    EXPECT_DOUBLE_EQ(0.5, primitive->getMaterial()->color.Y);
    EXPECT_DOUBLE_EQ(0.3, primitive->getMaterial()->color.Z);
}

TEST_F(APrimitiveTest, RotateX) {
    primitive->rotateX(45.0);
    EXPECT_DOUBLE_EQ(45.0, primitive->getRotationX());
    primitive->rotateX(-15.0);
    EXPECT_DOUBLE_EQ(30.0, primitive->getRotationX());
}

TEST_F(APrimitiveTest, RotateY) {
    primitive->rotateY(90.0);
    EXPECT_DOUBLE_EQ(90.0, primitive->getRotationY());
    primitive->rotateY(-30.0);
    EXPECT_DOUBLE_EQ(60.0, primitive->getRotationY());
}

TEST_F(APrimitiveTest, RotateZ) {
    primitive->rotateZ(180.0);
    EXPECT_DOUBLE_EQ(180.0, primitive->getRotationZ());
    primitive->rotateZ(-60.0);
    EXPECT_DOUBLE_EQ(120.0, primitive->getRotationZ());
}

TEST_F(APrimitiveTest, SourceFile) {
    primitive->setSourceFile("test_scene.cfg");
    EXPECT_EQ("test_scene.cfg", primitive->getSourceFile());
}

} // namespace RayTracerTest