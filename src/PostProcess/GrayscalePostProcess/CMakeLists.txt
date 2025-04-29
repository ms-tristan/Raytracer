file(GLOB GRAYSCALE_SOURCES "*.cpp")
file(GLOB GRAYSCALE_HEADERS "*.hpp")

add_library(grayscale_postprocess STATIC ${GRAYSCALE_SOURCES} ${GRAYSCALE_HEADERS})
set_property(TARGET grayscale_postprocess PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(grayscale_postprocess PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(grayscale_postprocess_plugin SHARED GrayscalePostProcessPlugin.cpp)
target_link_libraries(grayscale_postprocess_plugin PRIVATE grayscale_postprocess raytracer_core)
set_target_properties(grayscale_postprocess_plugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "grayscale_plugin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/postprocess")

install(TARGETS grayscale_postprocess_plugin
    LIBRARY DESTINATION plugins/postprocess)
