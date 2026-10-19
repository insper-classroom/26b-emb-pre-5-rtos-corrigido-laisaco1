add_executable(exe3
        main.c
)

target_link_libraries(exe3 pico_stdlib freertos)
pico_add_extra_outputs(exe3)
