find_package(Boost 1.71.0 REQUIRED)

file(GLOB db_header include/doorlose/database.hpp)
file(GLOB_RECURSE db_includes include/doorlose/database/*.hpp)
file(GLOB_RECURSE db_src src/*)

source_group("include\\doorlose" FILES ${db_header})
source_group("include\\doorlose\\database" FILES ${db_includes})
source_group("src"     FILES ${db_src})

add_library(doorlose_database SHARED ${db_header} ${db_includes} ${db_src})

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

file(GLOB_RECURSE db_test test/*.hpp test/*.cpp)
source_group("test" FILES ${db_test})

add_executable(doorlose_database_test ${db_test})

target_link_libraries(doorlose_database_test
    PRIVATE
        doorlose_database
        GTest::GTest
        GTest::Main
        Boost::boost #TODO: maybe not needed
)