set(INCROOT ${PROJECT_SOURCE_DIR}/src/Shader)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Shader)

add_subdirectory(Plugin)
add_subdirectory(PhongShader)
add_subdirectory(ToonShader)
add_subdirectory(FresnelShader)

set(SRC
    ${SRCROOT}/AShader.cpp
    ${SRCROOT}/ShaderFactory.cpp
    ${SRCROOT}/BasicShader.cpp
    ${SRCROOT}/ShaderDecorator/ShaderDecorator.cpp
    ${SRCROOT}/ShaderDecorator/FresnelDecorator.cpp
    ${SHADER_PLUGIN_SOURCES}
    ${PHONG_SHADER_SOURCES}
    ${TOON_SHADER_SOURCES}
    ${FRESNEL_SHADER_SOURCES}
)

add_library(shader STATIC ${SRC})
target_include_directories(shader PRIVATE ${PROJECT_SOURCE_DIR}/src)
