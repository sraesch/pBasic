cmake_minimum_required (VERSION 3.0)
project(pBasicTests)

enable_testing()

add_subdirectory(testVersion)
add_subdirectory(testVector)

add_test( testVersion testVersion )
add_test( testVector testVector )