set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE # Suppress CMP0135 warning
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # Disable clang-tidy for googletest
set(CMAKE_CXX_FORMAT "" CACHE STRING "" FORCE)     # Disable clang-format for googletest
FetchContent_MakeAvailable(googletest)

# Exclude googletest from clang-tidy checks
if (TARGET gtest)
    set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
endif()
if (TARGET gtest_main)
    set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
endif()

# Disable treating warnings as errors for googletest
if (TARGET gtest)
    target_compile_options(gtest PRIVATE -Wno-error)
endif()
if (TARGET gtest_main)
    target_compile_options(gtest_main PRIVATE -Wno-error)
endif()

add_executable(my_tests
    test_temp.cpp
)

target_link_libraries(my_tests PRIVATE ${LIB} gtest gtest_main)
include(GoogleTest)
gtest_discover_tests(my_tests)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(my_tests PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage -fno-inline)
    target_link_libraries(my_tests PRIVATE ${LIB} --coverage)
    add_custom_target(coverage
        COMMAND gcovr --root=${CMAKE_SOURCE_DIR} --object-directory=${CMAKE_BINARY_DIR}/tests/CMakeFiles/my_tests.dir/ --exclude ${CMAKE_SOURCE_DIR}/tests --exclude ${CMAKE_BINARY_DIR} --exclude ${CMAKE_SOURCE_DIR}/Documentation
        COMMENT "Displaying code coverage in terminal..."
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
