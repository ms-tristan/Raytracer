file(GLOB SUPERSAMPLING_SOURCES "*.cpp")
file(GLOB SUPERSAMPLING_HEADERS "*.hpp")

add_library(supersampling_postprocess STATIC SupersamplingPostProcess.cpp)
set_property(TARGET supersampling_postprocess PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(supersampling_postprocess PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_library(supersampling_postprocess_plugin SHARED ${SUPERSAMPLING_SOURCES})
target_link_libraries(supersampling_postprocess_plugin PRIVATE raytracer_core)
set_target_properties(supersampling_postprocess_plugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "supersampling_plugin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/postprocess")

install(TARGETS supersampling_postprocess_plugin
    LIBRARY DESTINATION plugins/postprocess)