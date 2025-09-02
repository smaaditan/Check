cmake_minimum_required(VERSION 3.10)
project(sample_ecu_demo C)

set(CMAKE_C_STANDARD 11)

# All source files
add_executable(sample_ecu_demo
    main.c
    a2l_generator.c
)
