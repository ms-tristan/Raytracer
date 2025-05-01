set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Initialize the FRACTALTYPE_SOURCES variable
set(FRACTALTYPE_SOURCES "")

# Add FractalType subdirectory to get the file list
add_subdirectory(FractalType)

# Create fractal_plugin with explicit source files
add_library(fractal_plugin SHARED
    FractalPlugin.cpp
    Fractal.cpp
    ${FRACTALTYPE_SOURCES}
)

target_include_directories(fractal_plugin PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Install in the plugins directory
set_target_properties(fractal_plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/primitives
    PREFIX ""
    SUFFIX ".so"
    OUTPUT_NAME "fractal_plugin"
)

# Add post-build command to copy the plugin to the plugins directory
add_custom_command(TARGET fractal_plugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/plugins/primitives
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fractal_plugin> ${CMAKE_SOURCE_DIR}/plugins/primitives/
    COMMENT "Copying fractal_plugin.so to ${CMAKE_SOURCE_DIR}/plugins/primitives/"
)