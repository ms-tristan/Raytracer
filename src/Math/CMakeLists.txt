add_subdirectory(Point3D)
add_subdirectory(Vector3D)

file(GLOB MATH_SOURCES "*.cpp")

# Combine all math sources
set(MATH_SOURCES
    ${MATH_SOURCES}
    ${POINT3D_SOURCES}
    ${VECTOR3D_SOURCES}
    PARENT_SCOPE
)