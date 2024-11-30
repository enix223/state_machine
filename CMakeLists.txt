cmake_minimum_required(VERSION 3.10.0)
project(state_machine VERSION 0.1.0 LANGUAGES C)

add_library(state_machine 
            src/linked_list.c
            src/state_machine.c
)

target_include_directories(state_machine PUBLIC inc)

include(CTest)
enable_testing()
