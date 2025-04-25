file(GLOB PRIMITIVE_FACTORY_SOURCES "*.cpp")

# Add the primitive factory to the main raytracer library
set(PRIMITIVE_FACTORY_CORE_SOURCES 
    PrimitiveFactory.cpp
)

set(PRIMITIVE_FACTORY_SOURCES ${PRIMITIVE_FACTORY_CORE_SOURCES} PARENT_SCOPE)