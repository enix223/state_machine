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

typedef enum
{
  POOL_STATE_FREE,
  POOL_STATE_OCCUPIED,
} pool_state;

#define NODE_POOL_SIZE (128)

static csm_linked_list_node_t node_pool[NODE_POOL_SIZE];
static unsigned char node_pool_state[16] = {POOL_STATE_FREE};

static csm_err_t malloc_node(csm_linked_list_node_t **node_ptr);

csm_err_t csm_linked_list_find_node(csm_linked_list_t *list, csm_comparator predicate, void *data)
{
  csm_linked_list_node_t *p = list->head;
  if (p == CSM_NULL)
  {
    return CSM_ERR_FAILED;
  }
  while (p != CSM_NULL)
  {
    if (predicate(p->data) == CSM_TRUE)
    {
      data = p->data;
      return CSM_ERR_OK;
    }
    p = p->next;
  }
  return CSM_ERR_FAILED;
}

csm_err_t csm_linked_list_append_node(csm_linked_list_t *list, void *data)
{
  csm_linked_list_node_t *new_node;
  csm_err_t ret = malloc_node(&new_node);
  if (ret == CSM_ERR_FAILED) {
    return ret;
  }

  new_node->data = data;
  new_node->next = CSM_NULL;

  if (list->head == CSM_NULL)
  {
    list->head = new_node;
  }
  else
  {
    list->head->next = new_node;
  }
  return CSM_ERR_OK;
}

csm_err_t csm_linked_list_remove_node(csm_linked_list_t *list, csm_comparator predicate)
{
  return CSM_ERR_OK;
}

static csm_err_t malloc_node(csm_linked_list_node_t **node_ptr)
{
  int j, k;
  pool_state state;
  for (int i = 0; i < NODE_POOL_SIZE; i++)
  {
    j = i % 8;
    k = i - j * 8;
    state = (node_pool_state[i] & (1 << k)) >> k;
    if (state == POOL_STATE_FREE)
    {
      node_pool_state[i] |= (1 << k);
      *node_ptr = &node_pool[i];
      return CSM_ERR_OK;
    }
  }
  return CSM_ERR_FAILED;
}