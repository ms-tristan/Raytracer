file(GLOB CYLINDER_SOURCES "*.cpp")

# Add the core Cylinder sources to the main raytracer library
set(CYLINDER_CORE_SOURCES
    Cylinder.cpp
)

# Create a shared library for the Cylinder plugin
add_library(cylinder_plugin SHARED CylinderPlugin.cpp Cylinder.cpp)
target_include_directories(cylinder_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(cylinder_plugin PRIVATE raytracer_core)
set_target_properties(cylinder_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(cylinder_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(cylinder_plugin PRIVATE --coverage)
endif()

# Add only core sources to the CYLINDER_SOURCES variable
set(CYLINDER_SOURCES ${CYLINDER_CORE_SOURCES} PARENT_SCOPE)