/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Temp test file
*/

#include <gtest/gtest.h>

TEST(SetupTest, BasicAssertion) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
