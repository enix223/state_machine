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

int test_state_machine_init_ok() {
  csm_state_machine_t machine;
  csm_machine_err_t ret = csm_machine_initialize(&machine, TEST_STATE_0);
  ASSERT_EQ(machine.current_state, TEST_STATE_0);
  ASSERT_EQ(machine.init_state, TEST_STATE_0);
  ASSERT_EQ(machine.internal_machine_status, TEST_STATE_0);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);
  return 0;
}

int main() {
  int ret = 0;
  ret |= test_state_machine_init_ok();
  return ret;
}
