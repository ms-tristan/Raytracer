cmake_minimum_required(VERSION 3.10)

set(COMMANDS_SOURCES)
set(COMMANDS_HEADERS
    ICommand.hpp
    MoveCommands.hpp
)

add_library(commands INTERFACE ${COMMANDS_SOURCES} ${COMMANDS_HEADERS})
target_include_directories(commands INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Set output directory
set_target_properties(commands PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)
