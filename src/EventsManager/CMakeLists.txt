##
## EPITECH PROJECT, 2025
## RayTracer
## File description:
## CMakeLists for EventsManager
##

# Add the Commands subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/InputManager/Commands)

set(EVENTS_MANAGER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/IEventsManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SFMLEventsManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SFMLEventsManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputManager/InputManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputManager/InputManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputManager/Commands/ICommand.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputManager/Commands/MoveCommands.hpp
)

add_library(eventsmanager STATIC ${EVENTS_MANAGER_SOURCES})
target_include_directories(eventsmanager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link SFML libraries
target_link_libraries(eventsmanager PUBLIC sfml-graphics sfml-window)