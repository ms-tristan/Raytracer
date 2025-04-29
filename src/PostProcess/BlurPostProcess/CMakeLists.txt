file(GLOB BLUR_SOURCES "*.cpp")
file(GLOB BLUR_HEADERS "*.hpp")

add_library(blur_postprocess STATIC ${BLUR_SOURCES} ${BLUR_HEADERS})

set_property(TARGET blur_postprocess PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(blur_postprocess PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(blur_postprocess_plugin SHARED BlurPostProcessPlugin.cpp)
target_link_libraries(blur_postprocess_plugin PRIVATE blur_postprocess raytracer_core)
set_target_properties(blur_postprocess_plugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "blur_plugin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/postprocess")

install(TARGETS blur_postprocess_plugin
    LIBRARY DESTINATION plugins/postprocess)
