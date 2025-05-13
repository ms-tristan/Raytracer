file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

# Ajout du sous-répertoire Plugin contenant l'infrastructure des plugins
add_subdirectory(Plugin)

# Ajout des différents effets de post-traitement comme plugins
add_subdirectory(GrayscalePostProcess)
add_subdirectory(NegativePostProcess)
add_subdirectory(BlurPostProcess)
add_subdirectory(ChromaticAberrationPostProcess)
add_subdirectory(SupersamplingPostProcess)

# Création d'une bibliothèque minimale contenant uniquement l'infrastructure de plugin
# et non les implémentations spécifiques des effets
add_library(postprocess STATIC ${SOURCES} ${HEADERS} ${POSTPROCESS_PLUGIN_SOURCES})
target_include_directories(postprocess PRIVATE ${CMAKE_SOURCE_DIR}/src)
