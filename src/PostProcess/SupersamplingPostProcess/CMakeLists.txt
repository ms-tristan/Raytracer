file(GLOB SUPERSAMPLING_SOURCES "*.cpp")
file(GLOB SUPERSAMPLING_HEADERS "*.hpp")

# Créer une bibliothèque statique pour le SupersamplingPostProcess
# utilisée uniquement par le plugin, pas directement par le core
add_library(supersampling_postprocess STATIC SupersamplingPostProcess.cpp)
# Ajouter le flag -fPIC pour permettre la liaison dans des bibliothèques partagées
set_property(TARGET supersampling_postprocess PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(supersampling_postprocess PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Créer une bibliothèque partagée pour le plugin SupersamplingPostProcess
# On utilise SUPERSAMPLING_SOURCES pour s'assurer que tous les fichiers source sont inclus
add_library(supersampling_postprocess_plugin SHARED ${SUPERSAMPLING_SOURCES})
target_link_libraries(supersampling_postprocess_plugin PRIVATE raytracer_core)
set_target_properties(supersampling_postprocess_plugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "supersampling_plugin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/postprocess")

# Installer le plugin dans le dossier plugins/postprocess
install(TARGETS supersampling_postprocess_plugin
    LIBRARY DESTINATION plugins/postprocess)