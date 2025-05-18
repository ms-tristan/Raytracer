file(GLOB INFINITECYLINDER_SOURCES "*.cpp")

# Add the core InfiniteCylinder sources to the main raytracer library
set(INFINITECYLINDER_CORE_SOURCES
    InfiniteCylinder.cpp
)

# Create a shared library for the InfiniteCylinder plugin
add_library(infinitecylinder_plugin SHARED InfiniteCylinderPlugin.cpp InfiniteCylinder.cpp)
target_include_directories(infinitecylinder_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(infinitecylinder_plugin PRIVATE raytracer_core)
set_target_properties(infinitecylinder_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(infinitecylinder_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(infinitecylinder_plugin PRIVATE --coverage)
endif()

# Add only core sources to the INFINITECYLINDER_SOURCES variable
set(INFINITECYLINDER_SOURCES ${INFINITECYLINDER_CORE_SOURCES} PARENT_SCOPE)