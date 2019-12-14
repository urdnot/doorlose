cmake_minimum_required (VERSION 3.6.2)
project (doorlose CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++latest")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug and Release configurations" FORCE) 

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/../external/debug)
list(APPEND CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/../external/debug)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/../external)
list(APPEND CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/../external)

# Treat warnings as errors
add_compile_options(/WX)

add_subdirectory (backend)
add_subdirectory (database)
add_subdirectory (experiments)