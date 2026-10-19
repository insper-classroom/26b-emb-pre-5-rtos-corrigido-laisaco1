add_executable(exe5
        main.c
)

target_link_libraries(exe5 pico_stdlib freertos)
pico_add_extra_outputs(exe5)
