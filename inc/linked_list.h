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
#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include "types.h"

typedef enum {
  CSM_ERR_LINKED_LIST_OK,
  CSM_ERR_LINKED_LIST_EMPTY,
  CSM_ERR_LINKED_LIST_NO_SPACE,
  CSM_ERR_LINKED_LIST_NOT_FOUND,
  CSM_ERR_LINKED_LIST_FAILED,
  CSM_ERR_LINKED_LIST_ILLEGAL_STATE,
} csm_linked_list_err_t;

typedef struct _csm_linked_list_node_t {
  void *data;
  struct _csm_linked_list_node_t *next;
} csm_linked_list_node_t;

typedef csm_bool (*csm_comparator)(void *current_data, void *data_to_find);

typedef struct {
  csm_linked_list_node_t *head;
} csm_linked_list_t;

/**
 * @brief Find the node with given predicate
 * @param list linked list
 * @param data if node found, then data equals to the node's data
 * @param predicate Predicate to find the node
 * @param predicate_data Predicate context data (similar to closure context)
 * @param data Pointer to receive the data
 * @return CSM_ERR_OK if found, otherwise CSM_ERR_FAILED is returned
 */
csm_linked_list_err_t csm_linked_list_find_node(csm_linked_list_t *list, void *data, csm_comparator predicate,
                                                void *predicate_data);

/**
 * @brief Add a node to the end of the linked list
 * @param list linked list
 * @param data data to add to the linked list
 * @return CSM_ERR_OK if add success, otherwise CSM_ERR_FAILED is returned
 */
csm_linked_list_err_t csm_linked_list_append_node(csm_linked_list_t *list, void *data);

/**
 * @brief Remove a node from the linked list with given predicate
 * @param list linked list
 * @param predicate predicate to find the node to delete
 * @return CSM_ERR_OK if found, otherwise CSM_ERR_FAILED is returned
 */
csm_linked_list_err_t csm_linked_list_remove_node(csm_linked_list_t *list, csm_comparator predicate, void *data);

#endif /* LINKED_LIST_H_ */