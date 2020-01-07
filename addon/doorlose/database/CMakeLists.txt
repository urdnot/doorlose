find_package(Boost 1.71.0 REQUIRED)

file(GLOB db_header include/doorlose/database.hpp)
file(GLOB db_includes include/doorlose/database/*.hpp)
file(GLOB db_includes_detail include/doorlose/database/detail/*.hpp)
file(GLOB db_src src/*.hpp src/*.cpp)
file(GLOB db_src_detail src/detail/*.hpp src/detail/*.cpp)

source_group("include\\doorlose" FILES ${db_header})
source_group("include\\doorlose\\database" FILES ${db_includes})
source_group("include\\doorlose\\database\\detail" FILES ${db_includes_detail})
source_group("src"     FILES ${db_src})
source_group("src\\detail"     FILES ${db_src_detail})

add_library(doorlose_database SHARED ${db_header} ${db_includes} ${db_includes_detail} ${db_src} ${db_src_detail})

target_compile_definitions(doorlose_database
    PRIVATE
        DOORLOSE_DATABASE_BUILD
)

target_include_directories(doorlose_database
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(doorlose_database
    PRIVATE
        Boost::boost
)

#############################################################
# Database tests
#############################################################
find_package(GTest REQUIRED)

file(GLOB db_test test/*.hpp test/*.cpp)
file(GLOB db_test_detail test/detail/*.hpp test/detail/*.cpp)
source_group("test" FILES ${db_test})
source_group("test\\detail" FILES ${db_test_detail})

add_executable(doorlose_database_test ${db_test} ${db_test_detail})

target_link_libraries(doorlose_database_test
    PRIVATE
        doorlose_database
        GTest::GTest
        GTest::Main
        Boost::boost #TODO: maybe not needed
)