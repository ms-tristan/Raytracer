add_subdirectory(AmbientLight)
add_subdirectory(DirectionalLight)
add_subdirectory(ALight)
add_subdirectory(PointLight)
add_subdirectory(LightFactory)
add_subdirectory(LightDecorator)

file(GLOB LIGHT_SOURCES "*.cpp")