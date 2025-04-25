file(GLOB PLUGIN_SOURCES "*.cpp")

# Add the plugin loader to the main raytracer library
set(PLUGIN_CORE_SOURCES
    PrimitivePluginManager.cpp
    PluginLoader.cpp
)

set(PLUGIN_SOURCES ${PLUGIN_CORE_SOURCES} PARENT_SCOPE)