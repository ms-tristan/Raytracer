file(GLOB SPHERE_SOURCES "*.cpp")

# Add the core Sphere sources to the main raytracer library
set(SPHERE_CORE_SOURCES
    Sphere.cpp
)

# Create a shared library for the Sphere plugin
add_library(sphere_plugin SHARED SpherePlugin.cpp Sphere.cpp)
target_include_directories(sphere_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(sphere_plugin PRIVATE raytracer_core)
set_target_properties(sphere_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(sphere_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(sphere_plugin PRIVATE --coverage)
endif()

# Add only core sources to the SPHERE_SOURCES variable
set(SPHERE_SOURCES ${SPHERE_CORE_SOURCES} PARENT_SCOPE)
