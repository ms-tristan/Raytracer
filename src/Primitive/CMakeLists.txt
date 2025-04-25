add_subdirectory(APrimitive)
add_subdirectory(CompositePrimitive)
add_subdirectory(PrimitiveDecorator)
add_subdirectory(PrimitiveFactory)
add_subdirectory(Sphere)
add_subdirectory(Plane)
add_subdirectory(Cylinder)
add_subdirectory(Cone)
add_subdirectory(Plugin)
add_subdirectory(Box)

# List primitive source files directly
set(PRIMITIVE_LOCAL_SOURCES "")
file(GLOB PRIMITIVE_LOCAL_SOURCES "*.cpp")

# Set core source files from each subdirectory
set(PRIMITIVE_SOURCES
    ${PRIMITIVE_LOCAL_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitiveFactory/PrimitiveFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin/PrimitivePluginManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin/PluginLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/APrimitive/APrimitive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CompositePrimitive/CompositePrimitive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitiveDecorator/PrimitiveDecorator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Sphere/Sphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Plane/Plane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Cylinder/Cylinder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Cone/Cone.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Box/Box.cpp
    PARENT_SCOPE
)