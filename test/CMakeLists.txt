add_executable(statemachine_test
               linked_list_test.c
)

target_include_directories(statemachine_test
                           PRIVATE
                           ${CMAKE_SOURCE_DIR}/inc)

target_link_libraries(statemachine_test PRIVATE statemachine)

enable_testing()
add_test(
  NAME statemachine_test
  COMMAND $<TARGET_FILE:statemachine_test>
)
