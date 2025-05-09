set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # Disable clang-tidy for raytracer build

# Define include directories
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Make the include directories available to all targets
include_directories(${INCLUDE_DIRS})

# Add subdirectories for each component
add_subdirectory(Camera)
add_subdirectory(Exception)
add_subdirectory(IObserver)
add_subdirectory(Light)
add_subdirectory(Material)
add_subdirectory(Math)
add_subdirectory(Primitive)
add_subdirectory(Ray)
add_subdirectory(Rectangle3D)
add_subdirectory(Scene)
add_subdirectory(Transformation)
add_subdirectory(Renderer)
add_subdirectory(EventsManager)
add_subdirectory(Shader)
add_subdirectory(PostProcess)

file(GLOB MAIN_SOURCES "*.cpp")

# Core library sources - explicitly exclude primitive, shaders and postprocessing implementations
set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Material/Material.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Ray/Ray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Rectangle3D/Rectangle3D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene/Scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene/SceneBuilder/SceneBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene/SceneDirector/SceneDirector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Light/ALight/ALight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Light/AmbientLight/AmbientLight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Light/DirectionalLight/DirectionalLight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Light/PointLight/PointLight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/Point3D/Point3D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector3D/Vector3D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Transformation/Rotate/Rotate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Transformation/Translate/Translate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Transformation/Scale/Scale.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive/Plugin/PluginLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive/Plugin/PrimitivePluginManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive/PrimitiveDecorator/PrimitiveDecorator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive/APrimitive/APrimitive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive/PrimitiveFactory/PrimitiveFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Primitive/CompositePrimitive/CompositePrimitive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PostProcess/PostProcessFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PostProcess/Plugin/PostProcessPluginManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PostProcess/Plugin/PostProcessPluginLoader.cpp
)

# Explicitly exclude main.cpp from the core library
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*main\.cpp$")

# Create a shared core library with position-independent code
add_library(raytracer_core SHARED ${CORE_SOURCES})
set_target_properties(raytracer_core PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_include_directories(raytracer_core PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(raytracer_core PRIVATE dl shader postprocess)


# Create the main raytracer executable
add_executable(raytracer ${MAIN_SOURCES})
target_include_directories(raytracer PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(raytracer PRIVATE
    raytracer_core dl
    ${SFML_LIBRARIES}
    renderer
    eventsmanager
    scenedirector
    shader
    PkgConfig::LIBCONFIG
)


# Copy executable to root directory
add_custom_command(TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:raytracer> ${CMAKE_SOURCE_DIR}/raytracer
)