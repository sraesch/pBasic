cmake_minimum_required (VERSION 3.0)
project(Documentation)

set(PBASIC_DOC_BUILD_DIR "${CMAKE_BINARY_DIR}/doc")
set(PBASIC_DOC_SOURCE_DIR "${CMAKE_SOURCE_DIR}/pBasicLib")

include(DoxygenConfig)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

add_custom_target(${PROJECT_NAME} ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)