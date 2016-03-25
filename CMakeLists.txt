cmake_minimum_required (VERSION 3.0)
project(pBasicProject)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake)

option(BUILD_TESTS "Build tests" OFF)

include(CompilerConfig)

add_subdirectory(pBasicLib)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)