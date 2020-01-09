cmake_minimum_required(VERSION 3.9)

if (CMAKE_JS_VERSION)
    cmake_policy(SET CMP0042 NEW)
    set (CMAKE_CXX_STANDARD 11)

    project (backend)
    include_directories(${CMAKE_JS_INC})
    file(GLOB SOURCE_FILES "backend.cpp")
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
    target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

    # Include N-API wrappers
    execute_process(COMMAND node -p "require('node-addon-api').include"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE NODE_ADDON_API_DIR
            )
    string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
    string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
    target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

    # define NPI_VERSION
    add_definitions(-DNAPI_VERSION=3)
else()
    file(GLOB node_src ./*.cpp ./*.js ./*.json)
    
    source_group("src" ${node_src})
    
    add_custom_target(doorlose_backend 
        COMMAND
            cmake-js build -d ${CMAKE_CURRENT_SOURCE_DIR}
        SOURCES
            ${node_src}
    )
endif()