/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Main entry point for tests
*/

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

void printCoverageInfo() {
    std::cout << "=== Test Coverage Information ===" << std::endl;
    std::cout << "Running test suite for Raytracer project" << std::endl;
    std::cout << "Coverage reports can be generated using 'make coverage'" << std::endl;
    std::cout << "NOTE: Some tests for complex primitives may require attention:" << std::endl;
    std::cout << "  - InfiniteCylinderTest.TransformedCylinderTest: Normal calculation needs fixing" << std::endl;
    std::cout << "  - MobiusStrip and TangleCube tests: Currently disabled due to missing utils" << std::endl;
    std::cout << "=================================" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    printCoverageInfo();
    return RUN_ALL_TESTS();
}