cmake_minimum_required (VERSION 3.6.2)
project (doorlose CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++latest")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug and Release configurations" FORCE) 

set(CMAKE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/../external/boost/1_71_0;${CMAKE_INCLUDE_PATH}")
set(CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/../external/boost/1_71_0/libs;${CMAKE_LIBRARY_PATH}")
set(CMAKE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/../external/googletest;${CMAKE_INCLUDE_PATH}")
set(CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/../external/googletest/lib;${CMAKE_LIBRARY_PATH}")

add_subdirectory (backend)
add_subdirectory (database)
add_subdirectory (experiments)