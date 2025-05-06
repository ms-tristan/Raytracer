set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Initialize list for collecting all fractal type sources
set(FRACTALTYPE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/IFractalType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FractalTypeFactory.hpp
)

# Add all subdirectories
add_subdirectory(Mandelbrot)
add_subdirectory(Julia)
add_subdirectory(Mandelbox)
add_subdirectory(MengerSponge)
add_subdirectory(Sierpinski)
add_subdirectory(QuaternionJulia)

# Set source property for parent scope
set(FRACTALTYPE_SOURCES ${FRACTALTYPE_SOURCES} PARENT_SCOPE)