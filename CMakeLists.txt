cmake_minimum_required(VERSION 3.22)
project(Raytracer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_PPM "Enable PPM output" OFF)
option(ENABLE_DOCS "Enable documentation" OFF)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system sfml-audio)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED IMPORTED_TARGET libconfig++)

add_subdirectory(src)

list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*/build/.*")
list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*Plugin\.cpp$")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Logs)

add_custom_target(
    cpplint-check
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cpplint.py ${PROJECT_SOURCE_DIR}/src > ${CMAKE_CURRENT_SOURCE_DIR}/Logs/cpplint-reports.log 2>&1
    COMMENT "🔍 Running cpplint on all source files without exclude rules and saving to Logs/cpplint-reports.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

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