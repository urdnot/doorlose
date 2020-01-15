find_package(Boost 1.71.0 REQUIRED)

file(GLOB db_header include/addon/database.hpp)
file(GLOB db_includes include/addon/database/*.hpp)
file(GLOB db_includes_detail include/addon/database/detail/*.hpp)
file(GLOB db_src src/*.hpp src/*.cpp)
file(GLOB db_src_detail src/detail/*.hpp src/detail/*.cpp)

source_group("include\\addon" FILES ${db_header})
source_group("include\\addon\\database" FILES ${db_includes})
source_group("include\\addon\\database\\detail" FILES ${db_includes_detail})
source_group("src"     FILES ${db_src})
source_group("src\\detail"     FILES ${db_src_detail})

add_library(addon_database SHARED ${db_header} ${db_includes} ${db_includes_detail} ${db_src} ${db_src_detail})

target_compile_definitions(addon_database
    PRIVATE
        ADDON_DATABASE_BUILD
)

target_include_directories(addon_database
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(addon_database
    PRIVATE
        Boost::boost
)

set_target_properties(addon_database
    PROPERTIES
        FOLDER addon
)

add_dependencies(doorlose_backend copy_addon_binaries)

#############################################################
# Database tests
#############################################################
find_package(GTest REQUIRED)

file(GLOB db_test test/*.hpp test/*.cpp)
file(GLOB db_test_detail test/detail/*.hpp test/detail/*.cpp)
source_group("test" FILES ${db_test})
source_group("test\\detail" FILES ${db_test_detail})

add_executable(addon_database_test ${db_test} ${db_test_detail})
add_dependencies(copy_addon_binaries addon_database_test)

target_link_libraries(addon_database_test
    PRIVATE
        addon_database
        GTest::GTest
        GTest::Main
        Boost::boost #TODO: maybe not needed
)

set_target_properties(addon_database_test
    PROPERTIES
        FOLDER addon
)