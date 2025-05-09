# Exception module CMakeLists.txt

# All exceptions are header-only classes, so we don't need to build anything,
# but we include them in the project for IDE support
set(EXCEPTION_HEADERS
    IException.hpp
    BaseException.hpp
    ValueRangeException.hpp
    PrimitiveNotFoundException.hpp
    SceneImportException.hpp
    FileIOException.hpp
    ConfigParseException.hpp
    InvalidOperationException.hpp
)

# Add the headers to the project for IDE support
add_custom_target(exception_headers SOURCES ${EXCEPTION_HEADERS})
