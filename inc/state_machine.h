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
#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "linked_list.h"
#include "types.h"

typedef enum {
  CSM_MACHINE_STATUS_NEW,
  CSM_MACHINE_STATUS_STARTED,
  CSM_MACHINE_STATUS_STOPPED,
  CSM_MACHINE_STATUS_DESTROYED,
} csm_machine_status;

#ifndef CSM_STATE_COUNT
#define CSM_STATE_COUNT (20)
#endif

typedef int csm_transition_t;

typedef int csm_state_t;

typedef enum {
  CSM_MACHINE_ERR_OK,
  CSM_MACHINE_ERR_ILLEGAL_STATUS,
  CSM_MACHINE_ERR_ILLEGAL_TRANSITION,
  CSM_MACHINE_ERR_FAILED,
} csm_machine_err_t;

typedef void (*csm_machine_on_state_changed)(csm_state_t prev_state, csm_state_t new_state,
                                             csm_transition_t transition);

typedef void (*csm_machine_on_machine_status_changed)(csm_machine_status prev_status, csm_machine_status new_status);

typedef struct {
  // machine status
  csm_machine_status internal_machine_status;

  // initial state
  csm_state_t init_state;

  // current state
  csm_state_t current_state;

  // state transition linked list
  csm_linked_list_t state_transition_linked_list[CSM_STATE_COUNT];

  // machine state change callback
  csm_machine_on_state_changed on_state_changed;

  // machine internal status change callback
  csm_machine_on_machine_status_changed on_machine_status_changed;
} csm_state_machine_t;

csm_machine_err_t csm_machine_initialize(csm_state_machine_t *machine, csm_state_t init_state);

csm_machine_err_t csm_machine_define_state_transition(csm_state_machine_t *machine, csm_state_t state,
                                                      csm_transition_t transition);

csm_machine_err_t csm_machine_dealloc(csm_state_machine_t *machine);

csm_machine_err_t csm_machine_start(csm_state_machine_t *machine);

csm_machine_err_t csm_machine_transit(csm_state_machine_t *machine, csm_transition_t transition);

csm_machine_err_t csm_machine_stop(csm_state_machine_t *machine);

csm_machine_err_t csm_machine_reset(csm_state_machine_t *machine);

#endif /* STATE_MACHINE_H_ */
