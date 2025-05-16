file(GLOB TRIANGLE_SOURCES "*.cpp")

set(TRIANGLE_CORE_SOURCES
    Triangle.cpp
)

add_library(triangle_plugin SHARED TrianglePlugin.cpp Triangle.cpp)
target_include_directories(triangle_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(triangle_plugin PRIVATE raytracer_core)
set_target_properties(triangle_plugin PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives"
)

set(TRIANGLE_SOURCES ${TRIANGLE_CORE_SOURCES} PARENT_SCOPE)