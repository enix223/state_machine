cmake_minimum_required(VERSION 3.10.0)
project(statemachine VERSION 0.1.0 LANGUAGES C)

add_library(statemachine 
            src/linked_list.c
            src/state_machine.c
)

target_include_directories(statemachine PUBLIC inc)

include(CTest)
enable_testing()
