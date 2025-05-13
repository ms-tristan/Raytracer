file(GLOB RENDERER_SOURCES "*.cpp")

add_library(renderer STATIC ${RENDERER_SOURCES})
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(renderer PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(DisplayManager)