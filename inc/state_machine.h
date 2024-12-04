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

#ifdef __cplusplus
extern "C" {
#endif

#include "conf.h"
#include "linked_list.h"
#include "types.h"

#define CSM_STATE_COUNT CONFIG_STATE_COUNT

typedef enum {
  CSM_MACHINE_STATUS_NEW,
  CSM_MACHINE_STATUS_STARTED,
  CSM_MACHINE_STATUS_STOPPED,
  CSM_MACHINE_STATUS_DESTROYED,
} csm_machine_status;

typedef int csm_transition_t;

typedef int csm_state_t;

typedef enum {
  CSM_MACHINE_ERR_OK,
  CSM_MACHINE_ERR_ILLEGAL_STATUS,
  CSM_MACHINE_ERR_ILLEGAL_TRANSITION,
  CSM_MACHINE_ERR_FAILED,
} csm_machine_err_t;

typedef struct {
  // from which state
  csm_state_t from_state;

  // transition
  csm_transition_t transition;

  // transit to which state
  csm_state_t to_state;
} csm_state_transition_node_t;

typedef struct csm_state_machine_t csm_state_machine_t;

typedef void (*csm_machine_on_state_changed)(csm_state_machine_t *machine, csm_state_t prev_state,
                                             csm_state_t new_state);

typedef void (*csm_machine_on_machine_status_changed)(csm_state_machine_t *machine, csm_machine_status prev_status,
                                                      csm_machine_status new_status);

typedef struct csm_state_machine_t {
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

/**
 * @brief Initialize a state machine with given initial state
 * @param machine pointer to the state machine
 * @param init_state initial state
 * @return CSM_MACHINE_ERR_OK if operation success, otherwise CSM_MACHINE_ERR_FAILED is returned
 */
csm_machine_err_t csm_machine_initialize(csm_state_machine_t *machine, csm_state_t init_state);

/**
 * @brief Register state change callback
 * @param machine pointer to the state machine
 * @param init_state state change callback
 * @return CSM_MACHINE_ERR_OK if operation success, otherwise CSM_MACHINE_ERR_FAILED is returned
 */
csm_machine_err_t csm_machine_register_on_state_changed(csm_state_machine_t *machine,
                                                        csm_machine_on_state_changed on_state_changed);

/**
 * @brief Register machine internal status change callback
 * @param machine pointer to the state machine
 * @param init_state status change callback
 * @return CSM_MACHINE_ERR_OK if operation success, otherwise CSM_MACHINE_ERR_FAILED is returned
 */
csm_machine_err_t csm_machine_register_on_machine_status_changed(
    csm_state_machine_t *machine, csm_machine_on_machine_status_changed on_machine_status_changed);

/**
 * @brief Define state change transition
 * @param machine pointer to the state machine
 * @param transition_node transition action
 * @return CSM_MACHINE_ERR_OK: operation success
 *         CSM_MACHINE_ERR_ILLEGAL_STATUS: if machine not in 'new' status
 */
csm_machine_err_t csm_machine_define_state_transition(csm_state_machine_t *machine,
                                                      csm_state_transition_node_t *transition_node);

/**
 * @brief Deallocate the machine
 * @param machine pointer to the state machine
 * @return CSM_MACHINE_ERR_OK: operation success
 *         CSM_MACHINE_ERR_ILLEGAL_STATUS: if machine not in 'stopped' status
 */
csm_machine_err_t csm_machine_dealloc(csm_state_machine_t *machine);

/**
 * @brief Start a state machine
 * @param machine pointer to the state machine
 * @return CSM_MACHINE_ERR_OK: operation success
 *         CSM_MACHINE_ERR_ILLEGAL_STATUS: if machine not in 'new' status
 */
csm_machine_err_t csm_machine_start(csm_state_machine_t *machine);

/**
 * @brief Trigger a state transition
 * @param machine pointer to the state machine
 * @param transition the transition to trigger
 * @return CSM_MACHINE_ERR_OK: operation success
 *         CSM_MACHINE_ERR_ILLEGAL_STATUS: if machine not in 'started' status
 */
csm_machine_err_t csm_machine_transit(csm_state_machine_t *machine, csm_transition_t transition);

/**
 * @brief Stop a state machine
 * @param machine pointer to the state machine
 * @return CSM_MACHINE_ERR_OK: operation success
 *         CSM_MACHINE_ERR_ILLEGAL_STATUS: if machine not in 'started' status
 */
csm_machine_err_t csm_machine_stop(csm_state_machine_t *machine);

/**
 * @brief Reset a state machine
 * @param machine pointer to the state machine
 * @return CSM_MACHINE_ERR_OK: operation success
 *         CSM_MACHINE_ERR_ILLEGAL_STATUS: if machine not in 'started' or stopped status
 */
csm_machine_err_t csm_machine_reset(csm_state_machine_t *machine);

#ifdef __cplusplus
}
#endif

#endif /* STATE_MACHINE_H_ */
