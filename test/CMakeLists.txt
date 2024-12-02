add_executable(statemachine_test
               state_machine_test.c
)
add_executable(linked_list_test
               linked_list_test.c
)

target_include_directories(statemachine_test
                           PRIVATE
                           ${CMAKE_SOURCE_DIR}/inc)
target_include_directories(linked_list_test
                           PRIVATE
                           ${CMAKE_SOURCE_DIR}/inc)

target_link_libraries(statemachine_test PRIVATE statemachine)
target_link_libraries(linked_list_test PRIVATE statemachine)

enable_testing()
add_test(
  NAME statemachine_test
  COMMAND $<TARGET_FILE:statemachine_test>
)
add_test(
  NAME linked_list_test
  COMMAND $<TARGET_FILE:linked_list_test>
)
