add_subdirectory(AmbientLight)
add_subdirectory(DirectionalLight)
add_subdirectory(ALight)
add_subdirectory(PointLight)
add_subdirectory(LightFactory)
add_subdirectory(LightDecorator)

file(GLOB LIGHT_SOURCES "*.cpp")

# Combine all light sources
set(LIGHT_SOURCES
    ${LIGHT_SOURCES}
    ${AMBIENT_LIGHT_SOURCES}
    ${DIRECTIONAL_LIGHT_SOURCES}
    ${ALIGHT_SOURCES}
    ${POINT_LIGHT_SOURCES}
    ${LIGHT_FACTORY_SOURCES}
    ${LIGHT_DECORATOR_SOURCES}
    PARENT_SCOPE
)