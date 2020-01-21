cmake_minimum_required (VERSION 3.6.2)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++latest")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug and Release configurations" FORCE)

set(THIRDPARTY_ROOT "${CMAKE_SOURCE_DIR}/external")
list(APPEND CMAKE_PREFIX_PATH ${THIRDPARTY_ROOT})
list(APPEND CMAKE_FIND_ROOT_PATH ${THIRDPARTY_ROOT})
list(APPEND CMAKE_PREFIX_PATH ${THIRDPARTY_ROOT}/Debug)
list(APPEND CMAKE_FIND_ROOT_PATH ${THIRDPARTY_ROOT}/Debug)
list(APPEND CMAKE_PREFIX_PATH ${THIRDPARTY_ROOT}/Release)
list(APPEND CMAKE_FIND_ROOT_PATH ${THIRDPARTY_ROOT}/Release)

add_compile_options(
    # Treat warnings as errors
    /WX
    # nonstandard extension used : zero-sized array in struct/union
    /wd4200
    # 'A' needs to have dll-interface to be used by clients of class 'B'
    /wd4251
    # non DLL-interface class 'A' used as base for DLL-interface class 'B'
    /wd4275
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(configuration_suffix "$<CONFIG>")
set(source_dir ${THIRDPARTY_ROOT}/${configuration_suffix}/bin)
set(target_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${configuration_suffix})

add_custom_target(copy_addon_binaries ALL ${CMAKE_COMMAND} -E copy_directory
    ${source_dir}
    ${target_dir}
)

set_target_properties(copy_addon_binaries
    PROPERTIES
        FOLDER addon
)

add_subdirectory (database)
add_subdirectory (experiments)