add_subdirectory(SceneFileWatcher)

file(GLOB OBSERVER_SOURCES "*.cpp")

# Combine all observer sources
set(OBSERVER_SOURCES
    ${OBSERVER_SOURCES}
    ${SCENE_FILE_WATCHER_SOURCES}
    PARENT_SCOPE)