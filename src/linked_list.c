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

typedef enum {
  POOL_STATE_FREE,
  POOL_STATE_OCCUPIED,
} pool_state;

#ifndef NODE_POOL_SIZE
#define NODE_POOL_SIZE (128)
#endif

static csm_linked_list_node_t node_pool[NODE_POOL_SIZE];
static unsigned char node_pool_state[NODE_POOL_SIZE / 8] = {POOL_STATE_FREE};

static csm_linked_list_err_t malloc_node(csm_linked_list_node_t **node_ptr);
static csm_linked_list_err_t free_node(csm_linked_list_node_t *node);
static pool_state get_node_state(int i);
static void set_node_state_occupied(int i);
static void set_node_state_free(int i);

csm_linked_list_err_t csm_linked_list_find_node(csm_linked_list_t *list, void *data, csm_comparator predicate,
                                                void *predicate_data) {
  csm_linked_list_node_t *p = list->head;
  if (p == CSM_NULL) {
    return CSM_ERR_LINKED_LIST_EMPTY;
  }
  while (p != CSM_NULL) {
    if (predicate(p->data, predicate_data) == CSM_TRUE) {
      data = p->data;
      return CSM_ERR_LINKED_LIST_OK;
    }
    p = p->next;
  }
  return CSM_ERR_LINKED_LIST_NOT_FOUND;
}

csm_linked_list_err_t csm_linked_list_append_node(csm_linked_list_t *list, void *data) {
  csm_linked_list_node_t *new_node;
  csm_linked_list_err_t ret = malloc_node(&new_node);
  if (ret != CSM_ERR_LINKED_LIST_OK) {
    return ret;
  }

  new_node->data = data;
  new_node->next = CSM_NULL;

  if (list->head == CSM_NULL) {
    list->head = new_node;
  } else {
    list->head->next = new_node;
  }
  return CSM_ERR_LINKED_LIST_OK;
}

csm_linked_list_err_t csm_linked_list_remove_node(csm_linked_list_t *list, csm_comparator predicate, void *data) {
  csm_linked_list_node_t *ptr = list->head;
  if (ptr == CSM_NULL) {
    return CSM_ERR_LINKED_LIST_EMPTY;
  }

  csm_linked_list_node_t *prev = CSM_NULL;
  csm_linked_list_err_t ret;
  while (ptr != CSM_NULL) {
    if (predicate(ptr->data, data) == CSM_TRUE) {
      prev->next = ptr->next;
      ret = free_node(ptr);
      if (ret != CSM_ERR_LINKED_LIST_OK) {
        return ret;
      }
      return CSM_ERR_LINKED_LIST_OK;
    }
    prev = ptr;
    ptr = ptr->next;
  }
  return CSM_ERR_LINKED_LIST_NOT_FOUND;
}

static csm_linked_list_err_t malloc_node(csm_linked_list_node_t **node_ptr) {
  int j, k;
  pool_state state;
  for (int i = 0; i < NODE_POOL_SIZE; i++) {
    state = get_node_state(i);
    if (state == POOL_STATE_FREE) {
      set_node_state_occupied(i);
      *node_ptr = &node_pool[i];
      return CSM_ERR_LINKED_LIST_OK;
    }
  }
  return CSM_ERR_LINKED_LIST_NO_SPACE;
}

csm_linked_list_err_t free_node(csm_linked_list_node_t *node) {
  csm_linked_list_node_t *ptr;
  for (int i = 0; i < NODE_POOL_SIZE; i++) {
    if (&node_pool[i] == node) {
      if (get_node_state(i) == POOL_STATE_OCCUPIED) {
        set_node_state_free(i);
        return CSM_ERR_LINKED_LIST_OK;
      }
      return CSM_ERR_LINKED_LIST_ILLEGAL_STATE;
    }
  }
  return CSM_ERR_LINKED_LIST_NOT_FOUND;
}

pool_state get_node_state(int i) {
  int j = i % 8;
  int k = i - j * 8;
  return (node_pool_state[i] & (1 << k)) >> k;
}

inline void set_node_state_occupied(int i) {
  int j = i % 8;
  int k = i - j * 8;
  node_pool_state[i] |= (1 << k);
}

inline void set_node_state_free(int i) {
  int j = i % 8;
  int k = i - j * 8;
  node_pool_state[i] &= 0xff & (0 << k);
}
