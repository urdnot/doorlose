file(GLOB experimets_sources *.cpp)

add_executable(experiments ${experimets_sources})

set_target_properties(experiments
    PROPERTIES
        FOLDER addon
)