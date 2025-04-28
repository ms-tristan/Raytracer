set(INCROOT ${PROJECT_SOURCE_DIR}/src/Shader)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Shader)

set(SRC
    ${SRCROOT}/AShader.cpp
    ${SRCROOT}/ShaderFactory.cpp
    ${SRCROOT}/BasicShader.cpp
    ${SRCROOT}/PhongShader.cpp
    ${SRCROOT}/ToonShader.cpp
    ${SRCROOT}/FresnelShader.cpp
    ${SRCROOT}/ShaderDecorator/ShaderDecorator.cpp
    ${SRCROOT}/ShaderDecorator/FresnelDecorator.cpp
)

add_library(shader STATIC ${SRC})
target_include_directories(shader PRIVATE ${PROJECT_SOURCE_DIR}/src)