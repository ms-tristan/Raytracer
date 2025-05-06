file(GLOB TORUS_SOURCES "*.cpp")

# Add the core Torus sources to the main raytracer library
set(TORUS_CORE_SOURCES
    Torus.cpp
)

# Create a shared library for the Torus plugin
add_library(torus_plugin SHARED TorusPlugin.cpp Torus.cpp)
target_include_directories(torus_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(torus_plugin PRIVATE raytracer_core)
set_target_properties(torus_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add only core sources to the TORUS_SOURCES variable
set(TORUS_SOURCES ${TORUS_CORE_SOURCES} PARENT_SCOPE)