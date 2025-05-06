add_library(kleinbottle_plugin MODULE
    KleinBottle.cpp
    KleinBottlePlugin.cpp
)

target_include_directories(kleinbottle_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(kleinbottle_plugin PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives"
)

if(APPLE)
    set_target_properties(kleinbottle_plugin PROPERTIES SUFFIX ".dylib")
endif()