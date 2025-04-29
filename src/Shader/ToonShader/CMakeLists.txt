file(GLOB TOON_SHADER_SOURCES "*.cpp")

# Core sources needed for the main shader library
set(TOON_SHADER_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ToonShader.cpp
)

# Create a shared library for the ToonShader plugin
add_library(toon_shader_plugin SHARED ToonShaderPlugin.cpp)
target_include_directories(toon_shader_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(toon_shader_plugin PRIVATE raytracer_core)
set_target_properties(toon_shader_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/shaders")

# Add the core sources to the TOON_SHADER_SOURCES variable
set(TOON_SHADER_SOURCES ${TOON_SHADER_CORE_SOURCES} PARENT_SCOPE)