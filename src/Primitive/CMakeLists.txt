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
add_subdirectory(Torus)
add_subdirectory(MobiusStrip)
add_subdirectory(Fractal)
add_subdirectory(KleinBottle)
add_subdirectory(InfiniteCylinder)
add_subdirectory(TangleCube)


# List primitive source files directly
set(PRIMITIVE_LOCAL_SOURCES "")
file(GLOB PRIMITIVE_LOCAL_SOURCES "*.cpp")

# Only include core primitive infrastructure - not the actual primitive implementations
set(PRIMITIVE_SOURCES
    ${PRIMITIVE_LOCAL_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitiveFactory/PrimitiveFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin/PrimitivePluginManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin/PluginLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/APrimitive/APrimitive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CompositePrimitive/CompositePrimitive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitiveDecorator/PrimitiveDecorator.cpp
    PARENT_SCOPE
)