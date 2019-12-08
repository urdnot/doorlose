file(GLOB backend_sources *.cpp)

add_executable(backend ${backend_sources})

target_link_libraries(backend
    PRIVATE
        database
)