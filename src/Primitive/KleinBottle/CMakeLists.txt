add_library(kleinbottle_plugin MODULE
    KleinBottle.cpp
    KleinBottlePlugin.cpp
)

target_include_directories(kleinbottle_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(kleinbottle_plugin PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/primitives"
)

# Add coverage options if enabled
if(ENABLE_COVERAGE)
    target_compile_options(kleinbottle_plugin PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(kleinbottle_plugin PRIVATE --coverage)
endif()
