cmake_minimum_required (VERSION 3.6.2)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++latest")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug and Release configurations" FORCE)

set(THIRDPARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external")
list(APPEND CMAKE_PREFIX_PATH ${THIRDPARTY_ROOT})
list(APPEND CMAKE_FIND_ROOT_PATH ${THIRDPARTY_ROOT})
list(APPEND CMAKE_PREFIX_PATH ${THIRDPARTY_ROOT}/Debug)
list(APPEND CMAKE_FIND_ROOT_PATH ${THIRDPARTY_ROOT}/Debug)
list(APPEND CMAKE_PREFIX_PATH ${THIRDPARTY_ROOT}/Release)
list(APPEND CMAKE_FIND_ROOT_PATH ${THIRDPARTY_ROOT}/Release)

#add_compile_options(
#    # Treat warnings as errors
#    /WX
#    # nonstandard extension used : zero-sized array in struct/union
#    /wd4200
#    # 'A' needs to have dll-interface to be used by clients of class 'B'
#    /wd4251
#    # non DLL-interface class 'A' used as base for DLL-interface class 'B'
#    /wd4275
#)

set(configuration_suffix "$<CONFIG>")
set(ARTIFACTS_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../artifacts/backend/addon")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ARTIFACTS_OUTPUT_DIR}/${configuration_suffix}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ARTIFACTS_OUTPUT_DIR}/${configuration_suffix}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARTIFACTS_OUTPUT_DIR}/${configuration_suffix}/lib)

set(source_dir ${THIRDPARTY_ROOT}/${configuration_suffix}/bin)
set(target_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_custom_target(copy_thirdparty ALL ${CMAKE_COMMAND} -E copy_directory
    ${source_dir}
    ${target_dir}
)

set_target_properties(copy_thirdparty
    PROPERTIES
        FOLDER addon
)

add_subdirectory (database)
add_subdirectory (experiments)