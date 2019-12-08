find_package(Boost 1.71.0)
find_package(GTest)

file(GLOB database_includes include/*.hpp)
file(GLOB database_sources src/*.cpp)

add_library(database SHARED ${database_sources})

target_include_directories(database
    PRIVATE
        ${database_includes}
)

target_link_libraries (database
    PRIVATE
        Boost
)