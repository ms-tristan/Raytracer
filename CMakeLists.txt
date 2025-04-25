cmake_minimum_required(VERSION 3.22)
project(Raytracer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_DOCS "Enable documentation" OFF)

# Create plugins directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

# First, create a core library that will be used by both the main app and plugins
add_subdirectory(src)

# Exclude plugin implementation files from main build
list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*/build/.*")
list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*Plugin\.cpp$")

if (ENABLE_DOCS)
    add_subdirectory(docs)
endif()

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()