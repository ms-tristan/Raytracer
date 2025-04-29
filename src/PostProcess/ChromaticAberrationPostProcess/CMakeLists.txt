file(GLOB CHROMATIC_SOURCES "*.cpp")
file(GLOB CHROMATIC_HEADERS "*.hpp")

# Create a static library for the ChromaticAberrationPostProcess
add_library(chromatic_aberration_postprocess STATIC ${CHROMATIC_SOURCES} ${CHROMATIC_HEADERS})
# Add -fPIC flag to allow linking into shared libraries
set_property(TARGET chromatic_aberration_postprocess PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(chromatic_aberration_postprocess PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Create a shared library for the ChromaticAberrationPostProcess plugin
add_library(chromatic_aberration_postprocess_plugin SHARED ChromaticAberrationPostProcessPlugin.cpp)
target_link_libraries(chromatic_aberration_postprocess_plugin PRIVATE chromatic_aberration_postprocess raytracer_core)
set_target_properties(chromatic_aberration_postprocess_plugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "chromatic_aberration_plugin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/postprocess")

# Install the plugin to the plugins/postprocess directory
install(TARGETS chromatic_aberration_postprocess_plugin
    LIBRARY DESTINATION plugins/postprocess)