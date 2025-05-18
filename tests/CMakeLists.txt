set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) 
set(CMAKE_CXX_FORMAT "" CACHE STRING "" FORCE)     
FetchContent_MakeAvailable(googletest)

if (TARGET gtest)
    set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
endif()
if (TARGET gtest_main)
    set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
endif()

if (TARGET gtest)
    target_compile_options(gtest PRIVATE -Wno-error)
endif()
if (TARGET gtest_main)
    target_compile_options(gtest_main PRIVATE -Wno-error)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED IMPORTED_TARGET libconfig++)

set(CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Math/Point3D/Point3D.cpp
    ${CMAKE_SOURCE_DIR}/src/Math/Vector3D/Vector3D.cpp
    ${CMAKE_SOURCE_DIR}/src/Math/Matrix3x3/Matrix3x3.cpp
    ${CMAKE_SOURCE_DIR}/src/Ray/Ray.cpp
    ${CMAKE_SOURCE_DIR}/src/Material/Material.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/APrimitive/APrimitive.cpp
    ${CMAKE_SOURCE_DIR}/src/Transformation/Rotate/Rotate.cpp
)

set(PRIMITIVES_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Primitive/Sphere/Sphere.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/Box/Box.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/Cylinder/Cylinder.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/Cone/Cone.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/Plane/Plane.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/Torus/Torus.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/InfiniteCone/InfiniteCone.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/InfiniteCylinder/InfiniteCylinder.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/KleinBottle/KleinBottle.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/MobiusStrip/MobiusStrip.cpp
    ${CMAKE_SOURCE_DIR}/src/Primitive/MobiusStrip/Utils/MobiusStripUtils.cpp
)

set(TEXTURE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Texture/ATexture.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture/ImageTexture/ImageTexture.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture/NormalMap/NormalMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture/DisplacementMap/DisplacementMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture/SpecularMap/SpecularMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture/AmbientOcclusionMap/AmbientOcclusionMap.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture/ProceduralTexture/ProceduralTexture.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture/ProceduralTexture/ChessboardTexture.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture/ProceduralTexture/PerlinNoiseTexture.cpp
)

set(TEST_SOURCES
    test_main.cpp
    test_sphere.cpp
    test_box.cpp
    test_cylinder.cpp
    test_cone.cpp
    test_plane.cpp
    test_torus.cpp
    test_material.cpp
    test_imagetexture.cpp
    test_perlinnoisetexture.cpp
    test_aprimitive.cpp
    test_vector3d.cpp
    test_point3d.cpp
    test_matrix3x3.cpp
    test_vector2d.cpp
    test_normalmap.cpp
    test_displacementmap.cpp
    test_specularmap.cpp
    test_infinitecone.cpp
    test_infinitecylinder.cpp
    test_kleinbottle.cpp
    test_mobiusstrip.cpp
    test_mobiusstriputils.cpp
)

add_executable(my_tests
    ${TEST_SOURCES}
    ${CORE_SOURCES}
    ${PRIMITIVES_SOURCES}
    ${TEXTURE_SOURCES}
)

target_include_directories(my_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_definitions(my_tests PRIVATE -DTEST_ENVIRONMENT)

target_link_libraries(my_tests PRIVATE 
    gtest 
    gtest_main
    pthread
    PkgConfig::LIBCONFIG
)

include(GoogleTest)
gtest_discover_tests(my_tests)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(my_tests PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage -fno-inline)
    target_link_libraries(my_tests PRIVATE ${LIB} --coverage)
endif()
