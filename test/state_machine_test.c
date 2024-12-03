/*
 *  The MIT License (MIT)
 * Copyright (c) 2024 Enix Yu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "state_machine.h"

#include "assert.h"

typedef enum {
  TEST_STATE_0,
  TEST_STATE_1,
  TEST_STATE_2,
  TEST_STATE_3,
} test_state;

typedef enum {
  TEST_TRANSITION_A,
  TEST_TRANSITION_B,
  TEST_TRANSITION_C,
} test_transition;

int test_state_machine_init_ok() {
  csm_state_machine_t machine;
  csm_machine_err_t ret = csm_machine_initialize(&machine, TEST_STATE_0);
  ASSERT_EQ(machine.current_state, TEST_STATE_0);
  ASSERT_EQ(machine.init_state, TEST_STATE_0);
  ASSERT_EQ(machine.internal_machine_status, CSM_MACHINE_STATUS_NEW);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);
  return 0;
}

int test_state_machine_start_ok() {
  csm_state_machine_t machine;
  csm_machine_err_t ret = csm_machine_initialize(&machine, TEST_STATE_0);
  ret = csm_machine_start(&machine);
  ASSERT_EQ(machine.internal_machine_status, CSM_MACHINE_STATUS_STARTED);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);
  return 0;
}

int test_state_machine_stop_should_failed_if_machine_not_started() {
  csm_state_machine_t machine;
  csm_machine_err_t ret = csm_machine_initialize(&machine, TEST_STATE_0);
  ret = csm_machine_stop(&machine);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_ILLEGAL_STATUS);
  ASSERT_EQ(machine.internal_machine_status, CSM_MACHINE_STATUS_NEW);
  return 0;
}

int test_state_machine_transit_should_failed_if_machine_not_started() {
  csm_state_machine_t machine;
  csm_machine_err_t ret = csm_machine_initialize(&machine, TEST_STATE_0);
  ret = csm_machine_transit(&machine, TEST_TRANSITION_A);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_ILLEGAL_STATUS);
  ASSERT_EQ(machine.internal_machine_status, CSM_MACHINE_STATUS_NEW);
  ASSERT_EQ(machine.current_state, TEST_STATE_0);
  return 0;
}

int test_state_machine_should_transit_ok() {
  csm_state_machine_t machine;
  csm_machine_err_t ret = csm_machine_initialize(&machine, TEST_STATE_0);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);

  csm_state_transition_node_t trans_node1 = {
      .from_state = TEST_STATE_0,
      .transition = TEST_TRANSITION_A,
      .to_state = TEST_STATE_1,
  };
  ret = csm_machine_define_state_transition(&machine, &trans_node1);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);

  csm_state_transition_node_t trans_node2 = {
      .from_state = TEST_STATE_1,
      .transition = TEST_TRANSITION_B,
      .to_state = TEST_STATE_2,
  };
  ret = csm_machine_define_state_transition(&machine, &trans_node2);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);

  csm_state_transition_node_t trans_node3 = {
      .from_state = TEST_STATE_2,
      .transition = TEST_TRANSITION_C,
      .to_state = TEST_STATE_3,
  };
  ret = csm_machine_define_state_transition(&machine, &trans_node3);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);

  csm_state_transition_node_t trans_node4 = {
      .from_state = TEST_STATE_2,
      .transition = TEST_TRANSITION_A,
      .to_state = TEST_STATE_0,
  };
  ret = csm_machine_define_state_transition(&machine, &trans_node4);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);

  ret = csm_machine_start(&machine);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);

  ret = csm_machine_transit(&machine, TEST_TRANSITION_A);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);
  ASSERT_EQ(machine.current_state, TEST_STATE_1);
  ret = csm_machine_transit(&machine, TEST_TRANSITION_B);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);
  ASSERT_EQ(machine.current_state, TEST_STATE_2);
  ret = csm_machine_transit(&machine, TEST_TRANSITION_C);
  ASSERT_EQ(ret, CSM_MACHINE_ERR_OK);
  ASSERT_EQ(machine.current_state, TEST_STATE_3);
  return 0;
}

int main() {
  int ret = 0;
  ret |= test_state_machine_init_ok();
  ret |= test_state_machine_start_ok();
  ret |= test_state_machine_should_transit_ok();
  ret |= test_state_machine_stop_should_failed_if_machine_not_started();
  return ret;
}
