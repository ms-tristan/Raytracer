# Find Doxygen and required components
find_package(Doxygen REQUIRED dot)

# Set Doxygen configuration variables
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_LATEX YES)
set(DOXYGEN_GENERATE_MAN NO)
set(DOXYGEN_GENERATE_RTF NO)
set(DOXYGEN_GENERATE_XML NO)
set(DOXYGEN_GENERATE_QHP NO)
set(DOXYGEN_GENERATE_DOCBOOK NO)

# Set the output directory for documentation
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs")

# Specify input directory for Doxygen
set(DOXYGEN_INPUT "${CMAKE_SOURCE_DIR}/src")
set(DOXYGEN_RECURSIVE YES)

# Set LaTeX PDF generation options
set(DOXYGEN_LATEX_OUTPUT "latex")
set(DOXYGEN_PDFLATEX "YES")

# Set Doxygen UML options
set(DOXYGEN_HAVE_DOT "YES")
set(DOXYGEN_UML_LOOK "YES")
set(DOXYGEN_UML_LIMIT_NUM_FIELDS "50")
set(DOXYGEN_TEMPLATE_RELATIONS "YES")
set(DOXYGEN_CALL_GRAPH "YES")
set(DOXYGEN_CALLER_GRAPH "YES")
set(DOXYGEN_DOT_IMAGE_FORMAT "svg")
set(DOXYGEN_COLLABORATION_GRAPH "YES")
set(DOXYGEN_INCLUDE_GRAPH "YES")
set(DOXYGEN_INCLUDED_BY_GRAPH "YES")
set(DOXYGEN_INTERACTIVE_SVG "YES")

# Configure Doxygen command
doxygen_add_docs(
    docs
    ${DOXYGEN_INPUT}
    ALL
    COMMENT "Generating documentation for Raytracer project"
    COMMENT "Documentation generated in ${DOXYGEN_OUTPUT_DIRECTORY}"
    COMMENT "HTML documentation available in ${DOXYGEN_OUTPUT_DIRECTORY}/html/index.html"
)

# Custom command to generate PDF from LaTeX
add_custom_target(doc_pdf
    COMMAND ${CMAKE_COMMAND} -E chdir ${DOXYGEN_OUTPUT_DIRECTORY}/latex make
    DEPENDS docs
    COMMENT "Generating PDF documentation"
    COMMAND mv ${DOXYGEN_OUTPUT_DIRECTORY}/latex/refman.pdf ${CMAKE_SOURCE_DIR}/docs/RaytracerDoc.pdf
)
