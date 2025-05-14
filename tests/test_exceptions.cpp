/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Test file for Exception classes
*/

#include <gtest/gtest.h>
#include <string>
#include "Exception/BaseException.hpp"
#include "Exception/ConfigParseException.hpp"
#include "Exception/ValueRangeException.hpp"
#include "Exception/InvalidOperationException.hpp"
#include "Exception/PrimitiveNotFoundException.hpp"

namespace RayTracerTest {

TEST(ExceptionTest, BaseExceptionTest) {
    RayTracer::BaseException exStr("Test error message", 42);
    EXPECT_STREQ("Test error message", exStr.what());
    EXPECT_EQ(42, exStr.getErrorCode());
    RayTracer::BaseException exChar("Another test message", 123);
    EXPECT_STREQ("Another test message", exChar.what());
    EXPECT_EQ(123, exChar.getErrorCode());
    RayTracer::BaseException exDefault("Default error code");
    EXPECT_STREQ("Default error code", exDefault.what());
    EXPECT_EQ(84, exDefault.getErrorCode());
}

TEST(ExceptionTest, ConfigParseExceptionTest) {
    RayTracer::ConfigParseException cpEx("Missing field", "scene.cfg", 42);
    EXPECT_STREQ("Config error: Missing field in file 'scene.cfg' at line 42", cpEx.what());
    EXPECT_EQ(84, cpEx.getErrorCode());
    RayTracer::ConfigParseException cpExNoLine("Value out of range", "config.cfg");
    EXPECT_STREQ("Config error: Value out of range in file 'config.cfg'", cpExNoLine.what());
    EXPECT_EQ(84, cpExNoLine.getErrorCode());
}

TEST(ExceptionTest, ValueRangeExceptionTest) {
    RayTracer::ValueRangeException vrEx("color", 255, 0, 100);
    std::string expectedMessage = "Value for 'color' is 255, which is out of the acceptable range [0, 100]";
    EXPECT_STREQ(expectedMessage.c_str(), vrEx.what());
    EXPECT_EQ(84, vrEx.getErrorCode());
    RayTracer::ValueRangeException vrExMin("age", -5, 0);
    std::string expectedMinMessage = "Value for 'age' is -5, which is less than the minimum allowed value of 0";
    EXPECT_STREQ(expectedMinMessage.c_str(), vrExMin.what());
    RayTracer::ValueRangeException vrExMax("percentage", 120, 0, 100);
    std::string expectedMaxMessage = "Value for 'percentage' is 120, which is greater than the maximum allowed value of 100";
    EXPECT_STREQ(expectedMaxMessage.c_str(), vrExMax.what());
    RayTracer::ValueRangeException vrExInRange("score", 75, 0, 100);
    std::string expectedInRangeMessage = "Value for 'score' is 75, which is out of the acceptable range [0, 100]";
    EXPECT_STREQ(expectedInRangeMessage.c_str(), vrExInRange.what());
}

TEST(ExceptionTest, InvalidOperationExceptionTest) {
    RayTracer::InvalidOperationException ioEx("Cannot divide by zero");
    EXPECT_STREQ("Invalid operation: Cannot divide by zero", ioEx.what());
    EXPECT_EQ(84, ioEx.getErrorCode());
    RayTracer::InvalidOperationException ioExWithCode("Operation not permitted", 42);
    EXPECT_STREQ("Invalid operation: Operation not permitted", ioExWithCode.what());
    EXPECT_EQ(42, ioExWithCode.getErrorCode());
}

TEST(ExceptionTest, PrimitiveNotFoundExceptionTest) {
    RayTracer::PrimitiveNotFoundException pnfEx("custom_primitive");
    EXPECT_STREQ("Primitive not found: custom_primitive", pnfEx.what());
    EXPECT_EQ(84, pnfEx.getErrorCode());
    RayTracer::PrimitiveNotFoundException pnfExWithCode("missing_primitive", 42);
    EXPECT_STREQ("Primitive not found: missing_primitive", pnfExWithCode.what());
    EXPECT_EQ(42, pnfExWithCode.getErrorCode());
}

} // namespace RayTracerTest