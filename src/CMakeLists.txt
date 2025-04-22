set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # Disable clang-tidy for raytracer build
SET(SRCMAIN ${CMAKE_CURRENT_SOURCE_DIR})
SET(INCMAIN ${CMAKE_CURRENT_SOURCE_DIR})

SET(SRC
    ${SRCMAIN}/main.cpp
)

add_executable(raytracer ${SRC})

# Explicitly disable clang-tidy for the raytracer target
set_target_properties(raytracer PROPERTIES CXX_CLANG_TIDY "")

add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raytracer> ${CMAKE_SOURCE_DIR}/raytracer
)