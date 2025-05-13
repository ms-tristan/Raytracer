file(GLOB INFINITECONE_SOURCES "*.cpp")

# Add the core InfiniteCone sources to the main raytracer library
set(INFINITECONE_CORE_SOURCES
    InfiniteCone.cpp
)

# Create a shared library for the InfiniteCone plugin
add_library(infinitecone_plugin SHARED InfiniteConePlugin.cpp InfiniteCone.cpp)
target_include_directories(infinitecone_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(infinitecone_plugin PRIVATE raytracer_core)
set_target_properties(infinitecone_plugin PROPERTIES 
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives")

# Add only core sources to the INFINITECONE_SOURCES variable
set(INFINITECONE_SOURCES ${INFINITECONE_CORE_SOURCES} PARENT_SCOPE)