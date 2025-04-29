add_subdirectory(SceneBuilder)
add_subdirectory(SceneDirector)

file(GLOB SCENE_SOURCES "*.cpp")

# Combine all scene sources
set(SCENE_SOURCES
    ${SCENE_SOURCES}
    ${SCENE_BUILDER_SOURCES}
    ${SCENE_DIRECTOR_SOURCES}
    PARENT_SCOPE
)