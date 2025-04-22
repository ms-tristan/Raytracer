cmake_minimum_required(VERSION 3.22)
project(Raytracer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CLANG_TIDY_OPTIONS
    "clang-tidy;-warnings-as-errors=*;-checks=modernize-*,readability-*,performance-*,bugprone-*,-modernize-use-trailing-return-type,-clang-analyzer-security.*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay;-extra-arg=-std=c++20;-extra-arg-before=-Iinclude;-extra-arg-before=-DDEBUG"
)
set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_OPTIONS}")
option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_DOCS "Enable documentation" OFF)

add_subdirectory(src)

file(GLOB_RECURSE ALL_SOURCE_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
)

list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*/build/.*")

add_custom_target(
    format
    COMMAND clang-format -i --style='{BasedOnStyle: llvm, IndentWidth: 4, ColumnLimit: 100}' ${ALL_SOURCE_FILES}
    COMMENT "🔧 Applying clang-format to the entire project"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    format-check
    COMMAND clang-format --dry-run --Werror --style='{BasedOnStyle: llvm, IndentWidth: 4, ColumnLimit: 100}' ${ALL_SOURCE_FILES}
    COMMENT "🔍 Checking clang-format differences"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    format-diff
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/docs/clang-format/show_diff.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "🔍 Affichage détaillé des différences de formatage proposées par clang-format"
)

add_custom_target(
    clang-check
    COMMAND clang-tidy ${ALL_SOURCE_FILES} -checks=modernize-*,readability-*,performance-*,bugprone-*,-header-filter=.*,-clang-analyzer-security.*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay -warnings-as-errors=* -header-filter=.* -- -std=c++20 -Iinclude -DDEBUG
    COMMENT "🔍 Running clang-tidy on all source files with synchronized options"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if (ENABLE_DOCS)
    add_subdirectory(docs)
endif()

if (ENABLE_TESTS)
    enable_testing()
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
    add_subdirectory(tests)
endif()