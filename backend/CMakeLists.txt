cmake_minimum_required(VERSION 3.9)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa /std:c++latest")

#set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug and Release configurations" FORCE)

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

set(configuration_suffix "$<CONFIG>")
set(ARTIFACTS_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../artifacts/backend")

if (CMAKE_JS_VERSION)
    cmake_policy(SET CMP0042 NEW)
    set (CMAKE_CXX_STANDARD 11)

    project(backend)
    
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ARTIFACTS_OUTPUT_DIR}/${configuration_suffix}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ARTIFACTS_OUTPUT_DIR}/${configuration_suffix}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARTIFACTS_OUTPUT_DIR}/${configuration_suffix}/lib)
    
    file(GLOB SOURCE_FILES "backend.cpp")
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_JS_INC}
            ${CMAKE_CURRENT_SOURCE_DIR}/addon/database/include
    )
    
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${CMAKE_JS_LIB}
            ${ARTIFACTS_OUTPUT_DIR}/addon/${configuration_suffix}/lib/addon_database.lib
    )

    # Include N-API wrappers
    execute_process(COMMAND node -p "require('node-addon-api').include"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE NODE_ADDON_API_DIR
            )
    string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
    string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${NODE_ADDON_API_DIR}
    )

    # define NPI_VERSION
    #add_definitions(-DNAPI_VERSION=3)
    add_definitions(-DNAPI_EXPERIMENTAL)
    
else()
    project (backend)
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../build)

    file(GLOB node_src ./*.cpp ./*.json)
    
    source_group("src" ${node_src})
    
    add_custom_target(${PROJECT_NAME}
        COMMAND
            cmake-js build --directory ${CMAKE_SOURCE_DIR} --out ${CMAKE_SOURCE_DIR}/../build $<$<CONFIG:Debug>:--debug>
        SOURCES
            ${node_src}
    )
    
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ARTIFACTS_OUTPUT_DIR}/addon/${configuration_suffix}
            ${ARTIFACTS_OUTPUT_DIR}/${configuration_suffix}
    )
    
    add_custom_target(install_native_addon
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ARTIFACTS_OUTPUT_DIR}/${configuration_suffix}/bin
            ${CMAKE_SOURCE_DIR}/../out/${configuration_suffix}
    )
    
    add_dependencies(install_native_addon ${PROJECT_NAME})
    
    add_subdirectory(addon)
endif()