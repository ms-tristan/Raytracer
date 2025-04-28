cmake_minimum_required(VERSION 3.22)
project(Raytracer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_DOCS "Enable documentation" OFF)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

add_subdirectory(src)

list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*/build/.*")
list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*Plugin\.cpp$")

if (ENABLE_DOCS)
    add_subdirectory(docs)
endif()

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_custom_target(copy_plugins ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_BINARY_DIR}/plugins/
    ${CMAKE_SOURCE_DIR}/plugins/
    COMMENT "Copying plugins to root directory"
    DEPENDS raytracer
)