/*
** EPITECH PROJECT, 2025
** B-OOP-400 Raytracer
** File description:
** Main
*/
#include <iostream>

// Intentional clang-tidy error: mismatched parameter names
void exampleFunction(int param1, int param2);
void exampleFunction(int a, int b) { std::cout << a + b << std::endl; }

// Intentional cpplint error: missing namespace comment
namespace TestNamespace {
void testFunction() {
    // Intentional clang-tidy warning: use of 'auto' suggested
    int x = 42;
    std::cout << "Value: " << x << std::endl;
}
}  // namespace TestNamespace

int main() {
    // Intentional cpplint warning: line exceeds 100 characters
    std::cout << "This is a very long line that exceeds the 100-character limit to trigger a cpplint warning." << std::endl;

    // Intentional cpplint error: improper indentation
  exampleFunction(1, 2);

    // Intentional cpplint error: missing space around operators
    int result=1+2;

    // Intentional cpplint error: include not at the top of the file
    #include <vector>

    TestNamespace::testFunction();
    return 0;
}
