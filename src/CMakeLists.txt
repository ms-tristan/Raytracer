SET(SRCMAIN ${CMAKE_CURRENT_SOURCE_DIR})
SET(INCMAIN ${CMAKE_CURRENT_SOURCE_DIR})

SET(SRC
    ${SRCMAIN}/main.cpp
)

add_executable(raytracer ${SRC})

add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raytracer> ${CMAKE_SOURCE_DIR}/raytracer
)