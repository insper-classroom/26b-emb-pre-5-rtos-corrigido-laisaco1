add_executable(exe4
        main.c
)

target_link_libraries(exe4 pico_stdlib freertos)
pico_add_extra_outputs(exe4)
