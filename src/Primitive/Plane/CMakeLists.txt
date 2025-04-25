file(GLOB PLANE_SOURCES "*.cpp")

# Add the core Plane sources to the main raytracer library
set(PLANE_CORE_SOURCES
    Plane.cpp
)

# Create a shared library for the Plane plugin
add_library(plane_plugin SHARED PlanePlugin.cpp)
target_include_directories(plane_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(plane_plugin PRIVATE raytracer_core)
set_target_properties(plane_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")

# Add only core sources to the PLANE_SOURCES variable
set(PLANE_SOURCES ${PLANE_CORE_SOURCES} PARENT_SCOPE)