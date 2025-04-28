file(GLOB FRESNEL_SHADER_SOURCES "*.cpp")

# Core sources needed for the main shader library
set(FRESNEL_SHADER_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/FresnelShader.cpp
)

# Create a shared library for the FresnelShader plugin
add_library(fresnel_shader_plugin SHARED FresnelShaderPlugin.cpp)
target_include_directories(fresnel_shader_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(fresnel_shader_plugin PRIVATE raytracer_core)
set_target_properties(fresnel_shader_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/shaders")

# Add the core sources to the FRESNEL_SHADER_SOURCES variable
set(FRESNEL_SHADER_SOURCES ${FRESNEL_SHADER_CORE_SOURCES} PARENT_SCOPE)