file(GLOB TANGLECUBE_SOURCES "*.cpp")

# Add the core TangleCube sources to the main raytracer library
set(TANGLECUBE_CORE_SOURCES
    TangleCube.cpp
)

# Create a shared library for the TangleCube plugin
add_library(tanglecube_plugin SHARED TangleCubePlugin.cpp TangleCube.cpp)
target_include_directories(tanglecube_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tanglecube_plugin PRIVATE raytracer_core)
set_target_properties(tanglecube_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(tanglecube_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(tanglecube_plugin PRIVATE --coverage)
endif()

# Add only core sources to the TANGLECUBE_SOURCES variable
set(TANGLECUBE_SOURCES ${TANGLECUBE_CORE_SOURCES} PARENT_SCOPE)