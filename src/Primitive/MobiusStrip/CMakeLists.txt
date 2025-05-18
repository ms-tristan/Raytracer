file(GLOB Mobius_SOURCES "*.cpp")
file(GLOB Mobius_UTILS_SOURCES "Utils/*.cpp")

# Add the core Mobius sources to the main raytracer library
set(MOBIUS_CORE_SOURCES
    MobiusStrip.cpp
    ${Mobius_UTILS_SOURCES}
)

# Create a shared library for the Mobius plugin
add_library(Mobius_plugin SHARED MobiusStripPlugin.cpp MobiusStrip.cpp ${Mobius_UTILS_SOURCES})
target_include_directories(Mobius_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Mobius_plugin PRIVATE raytracer_core)
set_target_properties(Mobius_plugin PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(Mobius_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(Mobius_plugin PRIVATE --coverage)
endif()

# Add only core sources to the Mobius_SOURCES variable
set(Mobius_SOURCES ${MOBIUS_CORE_SOURCES} PARENT_SCOPE)