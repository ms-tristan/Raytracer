/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test for Matrix3x3 class
*/

#include "Math/Matrix3x3/Matrix3x3.hpp"
#include "Math/Vector3D/Vector3D.hpp"
#include "gtest/gtest.h"

namespace RayTracerTest {

class Matrix3x3Test : public ::testing::Test {
 protected:
    void SetUp() override {
        Math::Vector3D row1({1.0, 2.0, 3.0});
        Math::Vector3D row2({4.0, 5.0, 6.0});
        Math::Vector3D row3({7.0, 8.0, 9.0});
        customMatrix = Math::Matrix3x3(row1, row2, row3);

        Math::Vector3D id1({1.0, 0.0, 0.0});
        Math::Vector3D id2({0.0, 1.0, 0.0});
        Math::Vector3D id3({0.0, 0.0, 1.0});
        identity = Math::Matrix3x3(id1, id2, id3);
    }

    Math::Matrix3x3 customMatrix;
    Math::Matrix3x3 identity;
};

TEST_F(Matrix3x3Test, DefaultConstructorTest) {
    Math::Matrix3x3 matrix;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                EXPECT_DOUBLE_EQ(1.0, matrix.m[i][j]);
            } else {
                EXPECT_DOUBLE_EQ(0.0, matrix.m[i][j]);
            }
        }
    }
}

TEST_F(Matrix3x3Test, VectorConstructorTest) {
    Math::Vector3D row1({1.0, 2.0, 3.0});
    Math::Vector3D row2({4.0, 5.0, 6.0});
    Math::Vector3D row3({7.0, 8.0, 9.0});
    
    Math::Matrix3x3 matrix(row1, row2, row3);
    
    EXPECT_DOUBLE_EQ(1.0, matrix.m[0][0]);
    EXPECT_DOUBLE_EQ(2.0, matrix.m[0][1]);
    EXPECT_DOUBLE_EQ(3.0, matrix.m[0][2]);
    EXPECT_DOUBLE_EQ(4.0, matrix.m[1][0]);
    EXPECT_DOUBLE_EQ(5.0, matrix.m[1][1]);
    EXPECT_DOUBLE_EQ(6.0, matrix.m[1][2]);
    EXPECT_DOUBLE_EQ(7.0, matrix.m[2][0]);
    EXPECT_DOUBLE_EQ(8.0, matrix.m[2][1]);
    EXPECT_DOUBLE_EQ(9.0, matrix.m[2][2]);
}

TEST_F(Matrix3x3Test, VectorMultiplicationTest) {
    Math::Vector3D v({1.0, 2.0, 3.0});
    
    Math::Vector3D result = customMatrix * v;
    
    double expectedX = customMatrix.m[0][0] * v.X + customMatrix.m[0][1] * v.Y + customMatrix.m[0][2] * v.Z;
    double expectedY = customMatrix.m[1][0] * v.X + customMatrix.m[1][1] * v.Y + customMatrix.m[1][2] * v.Z;
    double expectedZ = customMatrix.m[2][0] * v.X + customMatrix.m[2][1] * v.Y + customMatrix.m[2][2] * v.Z;
    
    EXPECT_DOUBLE_EQ(expectedX, result.X);
    EXPECT_DOUBLE_EQ(expectedY, result.Y);
    EXPECT_DOUBLE_EQ(expectedZ, result.Z);
    
    Math::Vector3D identityResult = identity * v;
    EXPECT_DOUBLE_EQ(v.X, identityResult.X);
    EXPECT_DOUBLE_EQ(v.Y, identityResult.Y);
    EXPECT_DOUBLE_EQ(v.Z, identityResult.Z);
    
    Math::Vector3D zeroVec({0.0, 0.0, 0.0});
    Math::Vector3D zeroResult = customMatrix * zeroVec;
    EXPECT_DOUBLE_EQ(0.0, zeroResult.X);
    EXPECT_DOUBLE_EQ(0.0, zeroResult.Y);
    EXPECT_DOUBLE_EQ(0.0, zeroResult.Z);
    
    Math::Vector3D unitX({1.0, 0.0, 0.0});
    Math::Vector3D unitY({0.0, 1.0, 0.0});
    Math::Vector3D unitZ({0.0, 0.0, 1.0});
    
    Math::Vector3D resX = customMatrix * unitX;
    Math::Vector3D resY = customMatrix * unitY;
    Math::Vector3D resZ = customMatrix * unitZ;
    
    EXPECT_DOUBLE_EQ(customMatrix.m[0][0], resX.X);
    EXPECT_DOUBLE_EQ(customMatrix.m[1][0], resX.Y);
    EXPECT_DOUBLE_EQ(customMatrix.m[2][0], resX.Z);
    
    EXPECT_DOUBLE_EQ(customMatrix.m[0][1], resY.X);
    EXPECT_DOUBLE_EQ(customMatrix.m[1][1], resY.Y);
    EXPECT_DOUBLE_EQ(customMatrix.m[2][1], resY.Z);
    
    EXPECT_DOUBLE_EQ(customMatrix.m[0][2], resZ.X);
    EXPECT_DOUBLE_EQ(customMatrix.m[1][2], resZ.Y);
    EXPECT_DOUBLE_EQ(customMatrix.m[2][2], resZ.Z);
}

TEST_F(Matrix3x3Test, TransposeTest) {
    Math::Matrix3x3 transposed = customMatrix.transpose();
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(customMatrix.m[j][i], transposed.m[i][j]);
        }
    }
    
    Math::Matrix3x3 transposedIdentity = identity.transpose();
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(identity.m[i][j], transposedIdentity.m[i][j]);
        }
    }
    
    Math::Matrix3x3 doubleTransposed = transposed.transpose();
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(customMatrix.m[i][j], doubleTransposed.m[i][j]);
        }
    }
}

TEST_F(Matrix3x3Test, ExtremeValuesTest) {
    Math::Vector3D largeRow1({1e10, 2e10, 3e10});
    Math::Vector3D largeRow2({4e10, 5e10, 6e10});
    Math::Vector3D largeRow3({7e10, 8e10, 9e10});
    Math::Matrix3x3 largeMatrix(largeRow1, largeRow2, largeRow3);
    
    Math::Vector3D largeVec({1e5, 2e5, 3e5});
    Math::Vector3D largeResult = largeMatrix * largeVec;
    
    double expectedLargeX = largeMatrix.m[0][0] * largeVec.X + largeMatrix.m[0][1] * largeVec.Y + largeMatrix.m[0][2] * largeVec.Z;
    double expectedLargeY = largeMatrix.m[1][0] * largeVec.X + largeMatrix.m[1][1] * largeVec.Y + largeMatrix.m[1][2] * largeVec.Z;
    double expectedLargeZ = largeMatrix.m[2][0] * largeVec.X + largeMatrix.m[2][1] * largeVec.Y + largeMatrix.m[2][2] * largeVec.Z;
    
    EXPECT_DOUBLE_EQ(expectedLargeX, largeResult.X);
    EXPECT_DOUBLE_EQ(expectedLargeY, largeResult.Y);
    EXPECT_DOUBLE_EQ(expectedLargeZ, largeResult.Z);
    
    Math::Matrix3x3 largeTransposed = largeMatrix.transpose();
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(largeMatrix.m[j][i], largeTransposed.m[i][j]);
        }
    }
    
    Math::Vector3D smallRow1({1e-10, 2e-10, 3e-10});
    Math::Vector3D smallRow2({4e-10, 5e-10, 6e-10});
    Math::Vector3D smallRow3({7e-10, 8e-10, 9e-10});
    Math::Matrix3x3 smallMatrix(smallRow1, smallRow2, smallRow3);
    
    Math::Vector3D smallVec({1e-5, 2e-5, 3e-5});
    Math::Vector3D smallResult = smallMatrix * smallVec;
    
    double expectedSmallX = smallMatrix.m[0][0] * smallVec.X + smallMatrix.m[0][1] * smallVec.Y + smallMatrix.m[0][2] * smallVec.Z;
    double expectedSmallY = smallMatrix.m[1][0] * smallVec.X + smallMatrix.m[1][1] * smallVec.Y + smallMatrix.m[1][2] * smallVec.Z;
    double expectedSmallZ = smallMatrix.m[2][0] * smallVec.X + smallMatrix.m[2][1] * smallVec.Y + smallMatrix.m[2][2] * smallVec.Z;
    
    EXPECT_DOUBLE_EQ(expectedSmallX, smallResult.X);
    EXPECT_DOUBLE_EQ(expectedSmallY, smallResult.Y);
    EXPECT_DOUBLE_EQ(expectedSmallZ, smallResult.Z);
    
    Math::Matrix3x3 smallTransposed = smallMatrix.transpose();
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(smallMatrix.m[j][i], smallTransposed.m[i][j]);
        }
    }
}

TEST_F(Matrix3x3Test, VectorCopyTest) {
    Math::Vector3D row1({1.1, 2.2, 3.3});
    Math::Vector3D row2({4.4, 5.5, 6.6});
    Math::Vector3D row3({7.7, 8.8, 9.9});
    
    Math::Matrix3x3 matrix(row1, row2, row3);
    
    row1.X = 10.0;
    row2.Y = 20.0;
    row3.Z = 30.0;
    
    EXPECT_DOUBLE_EQ(1.1, matrix.m[0][0]);
    EXPECT_DOUBLE_EQ(2.2, matrix.m[0][1]);
    EXPECT_DOUBLE_EQ(3.3, matrix.m[0][2]);
    EXPECT_DOUBLE_EQ(4.4, matrix.m[1][0]);
    EXPECT_DOUBLE_EQ(5.5, matrix.m[1][1]);
    EXPECT_DOUBLE_EQ(6.6, matrix.m[1][2]);
    EXPECT_DOUBLE_EQ(7.7, matrix.m[2][0]);
    EXPECT_DOUBLE_EQ(8.8, matrix.m[2][1]);
    EXPECT_DOUBLE_EQ(9.9, matrix.m[2][2]);
}

TEST_F(Matrix3x3Test, SerializationFunctionalityTest) {
    Math::Vector3D row1({1.1, 2.2, 3.3});
    Math::Vector3D row2({4.4, 5.5, 6.6});
    Math::Vector3D row3({7.7, 8.8, 9.9});
    Math::Matrix3x3 original(row1, row2, row3);
    
    std::shared_ptr<Math::Matrix3x3> cloned = std::make_shared<Math::Matrix3x3>(original);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(original.m[i][j], cloned->m[i][j]);
        }
    }
    
    cloned->m[0][0] = 10.0;
    
    EXPECT_DOUBLE_EQ(1.1, original.m[0][0]);
    EXPECT_DOUBLE_EQ(10.0, cloned->m[0][0]);
}

TEST_F(Matrix3x3Test, SerializationFunctionalityTest2) {
    Math::Vector3D row1({1.1, 2.2, 3.3});
    Math::Vector3D row2({4.4, 5.5, 6.6});
    Math::Vector3D row3({7.7, 8.8, 9.9});
    Math::Matrix3x3 original(row1, row2, row3);
    
    std::shared_ptr<Math::Matrix3x3> cloned = std::make_shared<Math::Matrix3x3>(original);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(original.m[i][j], cloned->m[i][j]);
        }
    }
    
    cloned->m[0][0] = 10.0;
    
    EXPECT_DOUBLE_EQ(1.1, original.m[0][0]);
    EXPECT_DOUBLE_EQ(10.0, cloned->m[0][0]);
}

TEST_F(Matrix3x3Test, MatrixCloneTest) {
    Math::Matrix3x3* clonedMatrix = new Math::Matrix3x3(customMatrix);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(customMatrix.m[i][j], clonedMatrix->m[i][j]);
        }
    }
    
    clonedMatrix->m[0][0] = 99.0;
    EXPECT_DOUBLE_EQ(99.0, clonedMatrix->m[0][0]);
    EXPECT_DOUBLE_EQ(1.0, customMatrix.m[0][0]);
    
    delete clonedMatrix;
}

TEST_F(Matrix3x3Test, MatrixCopyTest) {
    Math::Matrix3x3 copiedMatrix(customMatrix);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(customMatrix.m[i][j], copiedMatrix.m[i][j]);
        }
    }
    
    copiedMatrix.m[1][1] = 42.0;
    EXPECT_DOUBLE_EQ(42.0, copiedMatrix.m[1][1]);
    EXPECT_DOUBLE_EQ(5.0, customMatrix.m[1][1]);
    
    Math::Matrix3x3 assignedMatrix;
    assignedMatrix = customMatrix;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(customMatrix.m[i][j], assignedMatrix.m[i][j]);
        }
    }
    
    assignedMatrix.m[2][2] = 77.0;
    EXPECT_DOUBLE_EQ(77.0, assignedMatrix.m[2][2]);
    EXPECT_DOUBLE_EQ(9.0, customMatrix.m[2][2]);
}

}  // namespace RayTracerTest