cmake_minimum_required (VERSION 3.0)
project(pBasic)

# ------------------- Source Files ------------------- #

file(GLOB_RECURSE SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp")
file(GLOB_RECURSE HDR RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "include/*.hpp")

source_group(Source   FILES ${SRC})
source_group(Header   FILES ${HDR})

# ------------------- Find Packages ------------------- #

include(BoostConfig)

# ---------------- Include Directories ---------------- #

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# ---------------- Add Target and install ---------------- #

add_library(${PROJECT_NAME} ${SRC} ${HDR})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY "include" DESTINATION ".")
