file(GLOB PHONG_SHADER_SOURCES "*.cpp")

set(PHONG_SHADER_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/PhongShader.cpp
)

add_library(phong_shader_plugin SHARED PhongShaderPlugin.cpp)
target_include_directories(phong_shader_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(phong_shader_plugin PRIVATE raytracer_core)
set_target_properties(phong_shader_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/shaders")

set(PHONG_SHADER_SOURCES ${PHONG_SHADER_CORE_SOURCES} PARENT_SCOPE)