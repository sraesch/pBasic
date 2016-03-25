# FindPBASIC
# --------
#
# Find the native pBasic includes and library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   PBASIC_INCLUDE_DIRS   - where to find pBasic/pBasic.hpp, etc.
#   PBASIC_LIBRARIES      - List of libraries when using pBasic.
#   PBASIC_FOUND          - True if pBasic found.
#
# Hints
# ^^^^^
#
# A user may set ``PBASIC_ROOT`` to a pBasic installation root to tell this
# module where to look.

set(_PBASIC_SEARCHES)

# Search PBASIC_ROOT first if it is set.
if(PBASIC_ROOT)
  set(_PBASIC_SEARCH_ROOT PATHS ${PBASIC_ROOT} NO_DEFAULT_PATH)
  list(APPEND _PBASIC_SEARCHES _PBASIC_SEARCH_ROOT)
endif()

set(PBASIC_NAMES pBasic)
set(PBASIC_NAMES_DEBUG pBasicd)

# Try each search configuration.
foreach(search ${_PBASIC_SEARCHES})
  find_path(PBASIC_INCLUDE_DIR NAMES pBasic.hpp ${${search}} PATH_SUFFIXES include)
endforeach()

# Allow PBASIC_LIBRARY to be set manually, as the location of the pBasic library
if(NOT PBASIC_LIBRARY)
  foreach(search ${_PBASIC_SEARCHES})
    find_library(PBASIC_LIBRARY_RELEASE NAMES ${PBASIC_NAMES} ${${search}} PATH_SUFFIXES lib)
    find_library(PBASIC_LIBRARY_DEBUG NAMES ${PBASIC_NAMES_DEBUG} ${${search}} PATH_SUFFIXES lib)
  endforeach()

  include(${CMAKE_CURRENT_LIST_DIR}/SelectLibraryConfigurations.cmake)
  select_library_configurations(PBASIC)
endif()

unset(PBASIC_NAMES)
unset(PBASIC_NAMES_DEBUG)

mark_as_advanced(PBASIC_LIBRARY PBASIC_INCLUDE_DIR)

# handle the QUIETLY and REQUIRED arguments and set PBASIC_FOUND to TRUE if
# all listed variables are TRUE
include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PBASIC REQUIRED_VARS PBASIC_LIBRARY PBASIC_INCLUDE_DIR)

if(PBASIC_FOUND)
    set(PBASIC_INCLUDE_DIRS ${PBASIC_INCLUDE_DIR})

    if(NOT PBASIC_LIBRARIES)
      set(PBASIC_LIBRARIES ${PBASIC_LIBRARY})
    endif()

    if(NOT TARGET PBASIC::PBASIC)
      add_library(PBASIC::PBASIC UNKNOWN IMPORTED)
      set_target_properties(PBASIC::PBASIC PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${PBASIC_INCLUDE_DIRS}")

      if(PBASIC_LIBRARY_RELEASE)
        set_property(TARGET PBASIC::PBASIC APPEND PROPERTY
          IMPORTED_CONFIGURATIONS RELEASE)
        set_target_properties(PBASIC::PBASIC PROPERTIES
          IMPORTED_LOCATION_RELEASE "${PBASIC_LIBRARY_RELEASE}")
      endif()

      if(PBASIC_LIBRARY_DEBUG)
        set_property(TARGET PBASIC::PBASIC APPEND PROPERTY
          IMPORTED_CONFIGURATIONS DEBUG)
        set_target_properties(PBASIC::PBASIC PROPERTIES
          IMPORTED_LOCATION_DEBUG "${PBASIC_LIBRARY_DEBUG}")
      endif()

      if(NOT PBASIC_LIBRARY_RELEASE AND NOT PBASIC_LIBRARY_DEBUG)
        set_property(TARGET PBASIC::PBASIC APPEND PROPERTY
          IMPORTED_LOCATION "${PBASIC_LIBRARY}")
      endif()
    endif()
endif()