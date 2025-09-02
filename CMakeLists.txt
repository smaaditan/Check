cmake_minimum_required(VERSION 3.10)
project(sample_ecu_demo C)

set(CMAKE_C_STANDARD 11)

add_executable(sample_ecu_demo
    src/main.c
    src/a2l_generator.c
)
