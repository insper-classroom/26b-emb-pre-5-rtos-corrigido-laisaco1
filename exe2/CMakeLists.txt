add_executable(exe2
        main.c
)

target_link_libraries(exe2 pico_stdlib freertos)
pico_add_extra_outputs(exe2)
