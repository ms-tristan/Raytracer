file(GLOB BOX_SOURCES "*.cpp")

# Add the core Box sources to the main raytracer library
set(BOX_CORE_SOURCES
    Box.cpp
)

# Create a shared library for the Box plugin
add_library(box_plugin SHARED BoxPlugin.cpp Box.cpp)
target_include_directories(box_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(box_plugin PRIVATE raytracer_core)
set_target_properties(box_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")

# Add only core sources to the BOX_SOURCES variable
set(BOX_SOURCES ${BOX_CORE_SOURCES} PARENT_SCOPE)