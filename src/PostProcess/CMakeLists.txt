file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

add_subdirectory(Plugin)
add_subdirectory(GrayscalePostProcess)
add_subdirectory(NegativePostProcess)
add_subdirectory(BlurPostProcess)
add_subdirectory(ChromaticAberrationPostProcess)

add_library(postprocess STATIC ${SOURCES} ${HEADERS} ${POSTPROCESS_PLUGIN_SOURCES})
target_include_directories(postprocess PRIVATE ${CMAKE_SOURCE_DIR}/src)
