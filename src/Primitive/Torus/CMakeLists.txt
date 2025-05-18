file(GLOB TORUS_SOURCES "*.cpp")

set(TORUS_CORE_SOURCES
    Torus.cpp
)

add_library(torus_plugin SHARED TorusPlugin.cpp Torus.cpp)
target_include_directories(torus_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(torus_plugin PRIVATE raytracer_core)
set_target_properties(torus_plugin PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives"
)

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(torus_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(torus_plugin PRIVATE --coverage)
endif()

set(TORUS_SOURCES ${TORUS_CORE_SOURCES} PARENT_SCOPE)