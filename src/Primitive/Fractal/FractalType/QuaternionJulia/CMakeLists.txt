set(QUATERNIONJULIA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/QuaternionJuliaFractal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QuaternionJuliaFractal.hpp
)

# Add sources to parent scope
set(FRACTALTYPE_SOURCES ${FRACTALTYPE_SOURCES} ${QUATERNIONJULIA_SOURCES} PARENT_SCOPE)