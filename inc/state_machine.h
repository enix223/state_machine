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

#include "types.h"

typedef enum {
  NEW,
  STARTED,
  STOPPED,
} csm_machine_status;

typedef struct {
  // current state
  int state;

  // machine status
  csm_machine_status status;
} csm_state_machine_t;

typedef int csm_transition_t;

typedef int csm_state_t;

csm_err_t csm_initialize_machine(csm_state_machine_t *machine);

csm_err_t csm_define_state_transition(csm_state_machine_t *machine, csm_state_t state, csm_transition_t transition);

csm_err_t csm_dealloc_machine(csm_state_machine_t *machine);

csm_err_t csm_start(csm_state_machine_t *machine);

csm_err_t csm_transit(csm_state_machine_t *machine, csm_transition_t transition);

csm_err_t csm_stop(csm_state_machine_t *machine);

csm_err_t csm_reset(csm_state_machine_t *machine);

#endif /* STATE_MACHINE_H_ */
