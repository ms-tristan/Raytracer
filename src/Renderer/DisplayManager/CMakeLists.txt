file(GLOB DISPLAY_MANAGER_SOURCES "*.cpp")

add_library(displaymanager STATIC ${DISPLAY_MANAGER_SOURCES})
target_include_directories(displaymanager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link against SFML libraries
target_link_libraries(displaymanager PRIVATE ${SFML_LIBRARIES})

# Link the renderer with displaymanager
target_link_libraries(renderer PUBLIC displaymanager)
