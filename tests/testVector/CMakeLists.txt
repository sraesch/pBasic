cmake_minimum_required (VERSION 3.0)
project(testVector)

# ------------------- Source Files ------------------- #

file(GLOB_RECURSE SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" "*.h")

# ------------------- Find Packages ------------------- #

include(BoostConfig)
include(pBasicLibConfig)

# ---------------- Include Directories ---------------- #

include_directories(${PASIC_INCLUDE_DIR})

# ---------------- Add Target and install ---------------- #

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${PBASIC_LIB_TARGET})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
