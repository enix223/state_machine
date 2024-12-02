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

#include "linked_list.h"

#include "assert.h"
#include "conf.h"

static csm_bool csm_int_predicate(void *current_data, void *data_to_find) {
  return *((int *)current_data) == *((int *)data_to_find) ? CSM_TRUE : CSM_FALSE;
}

int test_linked_list_append_should_ok() {
  csm_linked_list_t linked_list = {
      .head = CSM_NULL,
  };
  int data = 10;
  csm_linked_list_err_t ret = csm_linked_list_append_node(&linked_list, &data);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);

  csm_linked_list_remove_node(&linked_list, csm_int_predicate, &data);
  return 0;
}

int test_linked_list_append_and_find_should_ok() {
  csm_linked_list_t linked_list = {
      .head = CSM_NULL,
  };
  int data1 = 11;
  csm_linked_list_err_t ret = csm_linked_list_append_node(&linked_list, &data1);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);

  int data2 = 22;
  ret = csm_linked_list_append_node(&linked_list, &data2);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);

  int data3 = 33;
  ret = csm_linked_list_append_node(&linked_list, &data3);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);

  void *found;
  ret = csm_linked_list_find_node(&linked_list, &found, csm_int_predicate, &data1);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);
  ASSERT_EQ(*(int *)found, 11);

  ret = csm_linked_list_find_node(&linked_list, &found, csm_int_predicate, &data2);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);
  ASSERT_EQ(*(int *)found, 22);

  ret = csm_linked_list_find_node(&linked_list, &found, csm_int_predicate, &data3);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);
  ASSERT_EQ(*(int *)found, 33);

  int data4 = 99;
  ret = csm_linked_list_find_node(&linked_list, &found, csm_int_predicate, &data4);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_NOT_FOUND);

  csm_linked_list_remove_node(&linked_list, csm_int_predicate, &data1);
  csm_linked_list_remove_node(&linked_list, csm_int_predicate, &data2);
  csm_linked_list_remove_node(&linked_list, csm_int_predicate, &data3);
  return 0;
}

int test_linked_list_should_overflow_when_no_node_left() {
  csm_linked_list_t linked_list = {
      .head = CSM_NULL,
  };
  csm_linked_list_err_t ret;
  int i = 0;
  for (; i < CONFIG_NODE_POOL_SIZE; i++) {
    ret = csm_linked_list_append_node(&linked_list, &i);
    ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_OK);
  }
  ret = csm_linked_list_append_node(&linked_list, &i);
  ASSERT_EQ(ret, CSM_ERR_LINKED_LIST_NO_SPACE);
  return 0;
}

int main() {
  int ret = 0;
  ret |= test_linked_list_append_should_ok();
  ret |= test_linked_list_append_and_find_should_ok();
  ret |= test_linked_list_should_overflow_when_no_node_left();
  return ret;
}
