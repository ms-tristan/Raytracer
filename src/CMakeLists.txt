set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # Disable clang-tidy for raytracer build

# Define include directories
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Make the include directories available to all targets
include_directories(${INCLUDE_DIRS})

# Add subdirectories for each component
add_subdirectory(Math)
add_subdirectory(Primitive)
add_subdirectory(Scene)
add_subdirectory(Camera)
add_subdirectory(Material)
add_subdirectory(Light)
add_subdirectory(IObserver)
add_subdirectory(Ray)
add_subdirectory(Transformation)
add_subdirectory(Rectangle3D)
add_subdirectory(Renderer)
add_subdirectory(EventsManager)

# Collect all source files
file(GLOB_RECURSE RAYTRACER_SOURCES
    "*.cpp"
)

# Main executable
add_executable(raytracer ${RAYTRACER_SOURCES})
target_include_directories(raytracer PRIVATE ${INCLUDE_DIRS})

target_link_libraries(raytracer PRIVATE
    ${SFML_LIBRARIES}
    renderer
    eventsmanager
    PkgConfig::LIBCONFIG
)

# Explicitly disable clang-tidy for the raytracer target
set_target_properties(raytracer PROPERTIES CXX_CLANG_TIDY "")

# Copy executable to root directory
add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raytracer> ${CMAKE_SOURCE_DIR}/raytracer
)
