add_subdirectory(Rotate)
add_subdirectory(Translate)
add_subdirectory(Scale)

file(GLOB TRANSFORMATION_SOURCES "*.cpp")

# Combine all transformation sources
set(TRANSFORMATION_SOURCES
    ${TRANSFORMATION_SOURCES}
    ${ROTATE_SOURCES}
    ${TRANSLATE_SOURCES}
    ${SCALE_SOURCES}
    PARENT_SCOPE
)