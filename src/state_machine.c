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

#include "linked_list.h"

static inline csm_bool find_transition(void *current_data, void *data_to_find);

csm_machine_err_t csm_machine_initialize(csm_state_machine_t *machine, csm_state_t init_state) {
  machine->internal_machine_status = CSM_MACHINE_STATUS_NEW;
  machine->current_state = init_state;
  machine->init_state = init_state;
  for (int i = 0; i < CSM_STATE_COUNT; i++) {
    machine->state_transition_linked_list[i].head = CSM_NULL;
  }
  machine->on_machine_status_changed = CSM_NULL;
  machine->on_state_changed = CSM_NULL;
  return CSM_MACHINE_ERR_OK;
}

csm_machine_err_t csm_machine_register_on_state_changed(csm_state_machine_t *machine,
                                                        csm_machine_on_state_changed on_state_changed) {
  machine->on_state_changed = on_state_changed;
  return CSM_MACHINE_ERR_OK;
}

csm_machine_err_t csm_machine_register_on_machine_status_changed(
    csm_state_machine_t *machine, csm_machine_on_machine_status_changed on_machine_status_changed) {
  machine->on_machine_status_changed = on_machine_status_changed;
  return CSM_MACHINE_ERR_OK;
}

csm_machine_err_t csm_machine_define_state_transition(csm_state_machine_t *machine,
                                                      csm_state_transition_node_t *trans_node) {
  if (machine->internal_machine_status != CSM_MACHINE_STATUS_NEW) {
    // can only define transition when machine in new status
    return CSM_MACHINE_ERR_ILLEGAL_STATUS;
  }
  csm_linked_list_t *linked_list = &machine->state_transition_linked_list[trans_node->from_state];
  csm_linked_list_append_node(linked_list, trans_node);
  return CSM_MACHINE_ERR_OK;
}

csm_machine_err_t csm_machine_dealloc(csm_state_machine_t *machine) {
  if (machine->internal_machine_status != CSM_MACHINE_STATUS_STOPPED) {
    return CSM_MACHINE_ERR_ILLEGAL_STATUS;
  }
  machine->internal_machine_status = CSM_MACHINE_STATUS_DESTROYED;
  return CSM_MACHINE_ERR_OK;
}

csm_machine_err_t csm_machine_start(csm_state_machine_t *machine) {
  if (machine->internal_machine_status != CSM_MACHINE_STATUS_NEW) {
    return CSM_MACHINE_ERR_ILLEGAL_STATUS;
  }
  machine->internal_machine_status = CSM_MACHINE_STATUS_STARTED;
  if (machine->on_machine_status_changed != CSM_NULL) {
    machine->on_machine_status_changed(machine, CSM_MACHINE_STATUS_NEW, CSM_MACHINE_STATUS_STARTED);
  }
  return CSM_MACHINE_ERR_OK;
}

csm_machine_err_t csm_machine_transit(csm_state_machine_t *machine, csm_transition_t transition) {
  if (machine->internal_machine_status != CSM_MACHINE_STATUS_STARTED) {
    return CSM_MACHINE_ERR_ILLEGAL_STATUS;
  }
  csm_linked_list_t *linked_list = &machine->state_transition_linked_list[machine->current_state];

  csm_state_transition_node_t find_criteria = {
      .transition = transition,
  };
  csm_state_transition_node_t *found_node;
  csm_linked_list_err_t ret =
      csm_linked_list_find_node(linked_list, (void **)&found_node, find_transition, &find_criteria);
  if (ret != CSM_ERR_LINKED_LIST_OK) {
    return CSM_MACHINE_ERR_ILLEGAL_TRANSITION;
  }

  if (machine->on_state_changed != CSM_NULL) {
    // trigger state change callback
    machine->on_state_changed(machine, machine->current_state, found_node->to_state);
  }

  machine->current_state = found_node->to_state;
  return CSM_MACHINE_ERR_OK;
}

csm_machine_err_t csm_machine_stop(csm_state_machine_t *machine) {
  if (machine->internal_machine_status != CSM_MACHINE_STATUS_STARTED) {
    return CSM_MACHINE_ERR_ILLEGAL_STATUS;
  }
  if (machine->on_machine_status_changed != CSM_NULL) {
    machine->on_machine_status_changed(machine, CSM_MACHINE_STATUS_STARTED, CSM_MACHINE_STATUS_STOPPED);
  }
  machine->internal_machine_status = CSM_MACHINE_STATUS_STOPPED;
  return CSM_MACHINE_ERR_OK;
}

csm_machine_err_t csm_machine_reset(csm_state_machine_t *machine) {
  if (machine->internal_machine_status != CSM_MACHINE_STATUS_STARTED &&
      machine->internal_machine_status != CSM_MACHINE_STATUS_STOPPED) {
    return CSM_MACHINE_ERR_ILLEGAL_STATUS;
  }
  if (machine->on_machine_status_changed != CSM_NULL) {
    machine->on_machine_status_changed(machine, machine->internal_machine_status, CSM_MACHINE_STATUS_STARTED);
  }
  if (machine->on_state_changed != CSM_NULL) {
    machine->on_state_changed(machine, machine->current_state, machine->init_state);
  }
  machine->current_state = machine->init_state;
  machine->internal_machine_status = CSM_MACHINE_STATUS_STARTED;
  return CSM_MACHINE_ERR_OK;
}

static inline csm_bool find_transition(void *current_data, void *data_to_find) {
  return ((csm_state_transition_node_t *)current_data)->transition ==
         ((csm_state_transition_node_t *)data_to_find)->transition;
}
