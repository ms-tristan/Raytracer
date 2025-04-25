file(GLOB SPHERE_SOURCES "*.cpp")

# Add the core Sphere sources to the main raytracer library
set(SPHERE_CORE_SOURCES
    Sphere.cpp
)

# Create a shared library for the Sphere plugin
add_library(sphere_plugin SHARED SpherePlugin.cpp)
target_include_directories(sphere_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(sphere_plugin PRIVATE raytracer_core)
set_target_properties(sphere_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")

# Add only core sources to the SPHERE_SOURCES variable
set(SPHERE_SOURCES ${SPHERE_CORE_SOURCES} PARENT_SCOPE)