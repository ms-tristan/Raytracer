set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(FRACTALTYPE_SOURCES "")

add_subdirectory(FractalType)

add_library(fractal_plugin SHARED
    FractalPlugin.cpp
    Fractal.cpp
    ${FRACTALTYPE_SOURCES}
)

target_include_directories(fractal_plugin PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(fractal_plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/primitives
    PREFIX ""
    SUFFIX ".so"
    OUTPUT_NAME "fractal_plugin"
)

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(fractal_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(fractal_plugin PRIVATE --coverage)
endif()
