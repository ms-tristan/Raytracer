set(SIERPINSKI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/SierpinskiTetrahedronFractal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SierpinskiTetrahedronFractal.hpp
)

# Add sources to parent scope
set(FRACTALTYPE_SOURCES ${FRACTALTYPE_SOURCES} ${SIERPINSKI_SOURCES} PARENT_SCOPE)