file(GLOB CONE_SOURCES "*.cpp")

# Add the core Cone sources to the main raytracer library
set(CONE_CORE_SOURCES
    Cone.cpp
)

# Create a shared library for the Cone plugin
add_library(cone_plugin SHARED ConePlugin.cpp Cone.cpp)
target_include_directories(cone_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(cone_plugin PRIVATE raytracer_core)
set_target_properties(cone_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(cone_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(cone_plugin PRIVATE --coverage)
endif()

# Add only core sources to the CONE_SOURCES variable
set(CONE_SOURCES ${CONE_CORE_SOURCES} PARENT_SCOPE)