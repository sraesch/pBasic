if(BOOST_LIIBRARY_CONFIGURED)
    return()
endif(BOOST_LIIBRARY_CONFIGURED)

set(BOOST_LIIBRARY_CONFIGURED true)

set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)

add_definitions("-DBOOST_ALL_NO_LIB")

find_package(Boost 1.55.0 COMPONENTS thread)
include_directories(${Boost_INCLUDE_DIRS})
