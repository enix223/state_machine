add_library(statemachine 
            linked_list.c
            state_machine.c
)

target_include_directories(statemachine PUBLIC ${CMAKE_SOURCE_DIR}/inc)
