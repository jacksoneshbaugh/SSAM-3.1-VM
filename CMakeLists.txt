cmake_minimum_required(VERSION 3.29)
project(SSAM_3_1_VM C)

set(CMAKE_C_STANDARD 11)

add_executable(vm
        memory.c
        memory.h
        controller.c
        controller.h
        sim.c
        sim.h
)
