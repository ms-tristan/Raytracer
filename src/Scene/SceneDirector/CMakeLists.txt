file(GLOB SCENE_DIRECTOR_SOURCES "*.cpp")

add_library(scenedirector STATIC ${SCENE_DIRECTOR_SOURCES})
target_link_libraries(scenedirector PUBLIC PkgConfig::LIBCONFIG)