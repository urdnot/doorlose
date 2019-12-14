file(GLOB sources *.cpp)

add_executable(doorlose_backend ${sources})

target_link_libraries(doorlose_backend
    PRIVATE
        doorlose_database
)