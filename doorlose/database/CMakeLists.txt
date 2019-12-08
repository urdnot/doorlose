find_package(Boost 1.71.0 REQUIRED)
find_package(GTest)

file(GLOB_RECURSE db_includes include/*.hpp)
file(GLOB_RECURSE db_src src/*)

source_group("include" FILES ${db_includes})
source_group("src"     FILES ${db_src})

add_library(database SHARED ${db_includes} ${db_src})

target_include_directories(database
    PRIVATE
        ${db_includes}
)

target_link_libraries(database
    PRIVATE
        Boost
)