file(GLOB NEGATIVE_SOURCES "*.cpp")
file(GLOB NEGATIVE_HEADERS "*.hpp")

# Create a static library for the NegativePostProcess
add_library(negative_postprocess STATIC ${NEGATIVE_SOURCES} ${NEGATIVE_HEADERS})
# Add -fPIC flag to allow linking into shared libraries
set_property(TARGET negative_postprocess PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(negative_postprocess PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Create a shared library for the NegativePostProcess plugin
add_library(negative_postprocess_plugin SHARED NegativePostProcessPlugin.cpp)
target_link_libraries(negative_postprocess_plugin PRIVATE negative_postprocess raytracer_core)
set_target_properties(negative_postprocess_plugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "negative_plugin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/postprocess")

# Install the plugin to the plugins/postprocess directory
install(TARGETS negative_postprocess_plugin
    LIBRARY DESTINATION plugins/postprocess)