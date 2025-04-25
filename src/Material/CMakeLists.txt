add_subdirectory(MaterialDecorator)

file(GLOB MATERIAL_SOURCES "*.cpp")

# Combine all material sources
set(MATERIAL_SOURCES
    ${MATERIAL_SOURCES}
    ${MATERIAL_DECORATOR_SOURCES}
    PARENT_SCOPE
)