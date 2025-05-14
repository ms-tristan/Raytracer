cmake_minimum_required(VERSION 3.22)
project(Raytracer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_DOCS "Enable documentation" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "Coverage enabled")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        
        add_custom_target(coverage
            COMMAND ctest
            COMMAND lcov --directory ${CMAKE_BINARY_DIR} --capture --output-file ${CMAKE_BINARY_DIR}/coverage.info --ignore-errors mismatch --rc geninfo_unexecuted_blocks=1
            COMMAND lcov --remove ${CMAKE_BINARY_DIR}/coverage.info '/usr/*' '${CMAKE_BINARY_DIR}/_deps/*' --output-file ${CMAKE_BINARY_DIR}/coverage.info
            COMMAND genhtml ${CMAKE_BINARY_DIR}/coverage.info --output-directory ${CMAKE_BINARY_DIR}/coverage_report
            COMMENT "Generating code coverage report"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    else()
        message(WARNING "Coverage reporting is only available with GCC or Clang compilers")
    endif()
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system sfml-audio)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED IMPORTED_TARGET libconfig++)

add_subdirectory(src)

list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*/build/.*")
list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*Plugin\.cpp$")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/logs)

add_custom_target(
    cpplint-check
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cpplint.py ${PROJECT_SOURCE_DIR}/src > ${CMAKE_CURRENT_SOURCE_DIR}/logs/cpplint-reports.log 2>&1
    COMMENT "🔍 Running cpplint on all source files without exclude rules and saving to logs/cpplint-reports.log"
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