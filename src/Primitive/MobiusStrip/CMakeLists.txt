file(GLOB Mobius_SOURCES "*.cpp")

# Add the core Mobius sources to the main raytracer library
set(MOBIUS_CORE_SOURCES
    MobiusStrip.cpp
)

# Create a shared library for the Mobius plugin
add_library(Mobius_plugin SHARED MobiusStripPlugin.cpp MobiusStrip.cpp)
target_include_directories(Mobius_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Mobius_plugin PRIVATE raytracer_core)
set_target_properties(Mobius_plugin PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add only core sources to the Mobius_SOURCES variable
set(Mobius_SOURCES ${MOBIUS_CORE_SOURCES} PARENT_SCOPE)